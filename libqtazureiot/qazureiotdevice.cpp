/****************************************************************************
**
** Copyright (C) 2017 Erik Larsson
** Contact: karl.erik.larsson@gmail.com
**
** This file is part of the libqtazureiot library.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: https://www.gnu.org/licenses/lgpl-2.1.html.
**
****************************************************************************/

#include "qazureiotdevice.h"
#include "qazureiotdevice_p.h"

#include <QMetaMethod>
#include <QJsonObject>
#include <QDebug>

static void deviceTwinCallback(DEVICE_TWIN_UPDATE_STATE update_state,
                               const unsigned char* payLoad,
                               size_t size,
                               void* userContextCallback)
{
    QByteArray data((const char*)payLoad, size);
    QAzureIotDevicePrivate *obj = (QAzureIotDevicePrivate*)userContextCallback;
    if (obj) {
        obj->handleDeviceTwinCallback(update_state, data);
    }
}

static int deviceMethodCallback(const char* method_name,
                                const unsigned char* payload,
                                size_t size,
                                unsigned char** response,
                                size_t* resp_size,
                                void* userContextCallback)
{
    QAzureIotDevicePrivate *obj = (QAzureIotDevicePrivate*)userContextCallback;
    if (obj) {
        return obj->handleDeviceMethodCallback(method_name, payload, size,
                                               response, resp_size);
    }
    return -1;
}

static void deviceTwinReportedStateCallback(int status_code, void* userContextCallback)
{
    qDebug() << Q_FUNC_INFO << status_code;
}

static IOTHUBMESSAGE_DISPOSITION_RESULT receiveMessageCallback(IOTHUB_MESSAGE_HANDLE message,
                                                               void* userContextCallback)
{
    QAzureIotDevicePrivate *obj = (QAzureIotDevicePrivate*)userContextCallback;
    if (obj) {
        return (IOTHUBMESSAGE_DISPOSITION_RESULT)obj->handleReceiveMessageCallback(message);
    }
    return IOTHUBMESSAGE_REJECTED;
}

static void connectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result,
                                     IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason,
                                     void *userContextCallback)
{
    QAzureIotDevicePrivate *obj = (QAzureIotDevicePrivate*)userContextCallback;
    if (obj) {
        obj->handleConnectionStatusCallback(result, reason);
    }
}



QAzureIotDevicePrivate::QAzureIotDevicePrivate(QAzureIotDevice *q, QByteArray connectionString)
    : q_ptr(q)
{
    m_connection.setConnectionString(connectionString);
    m_connection.connectToHub();

    if (IoTHubClient_LL_SetDeviceTwinCallback(m_connection.connectionHandle(),
                                              &deviceTwinCallback,
                                              (void*)this) != IOTHUB_CLIENT_OK)
    {
        qDebug() << Q_FUNC_INFO << "ERROR: IoTHubClient_LL_SetDeviceTwinCallback() failed";
    }

    if (IoTHubClient_LL_SetDeviceMethodCallback(m_connection.connectionHandle(),
                                                &deviceMethodCallback,
                                                (void *)this) != IOTHUB_CLIENT_OK)
    {
        qDebug() << Q_FUNC_INFO << "ERROR: IoTHubClient_LL_SetDeviceMethodCallback failed";
    }

    if (IoTHubClient_LL_SetMessageCallback(m_connection.connectionHandle(),
                                           &receiveMessageCallback,
                                           (void*)this) != IOTHUB_CLIENT_OK)
    {
        qDebug() << Q_FUNC_INFO << "ERROR: IoTHubClient_LL_SetMessageCallback failed";
    }

    if (IoTHubClient_LL_SetConnectionStatusCallback(m_connection.connectionHandle(),
                                                    &connectionStatusCallback,
                                                    (void*)this) != IOTHUB_CLIENT_OK)
    {
        qDebug() << Q_FUNC_INFO << "ERROR: IoTHubClient_LL_SetConnectionStatusCallback failed";
    }
}

void QAzureIotDevicePrivate::handleDeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState, QByteArray payload)
{
    Q_Q(QAzureIotDevice);

    QJsonDocument json = QJsonDocument::fromJson(payload);
    qDebug() << Q_FUNC_INFO << json;

    if (json != m_deviceTwinRawData) {
        m_deviceTwinRawData = json;
        emit q->deviceTwinRawDataChanged();

        QJsonObject reportedObj = json.object()["reported"].toObject();
        if (m_deviceTwinReportedData.object() != reportedObj) {
            qDebug() << "Reported data changed" << m_deviceTwinReportedData << reportedObj;
            m_deviceTwinReportedData.setObject(reportedObj);
            emit q->deviceTwinReportedDataChanged();
        }

        QJsonObject desiredObj = json.object()["desired"].toObject();
        if (m_deviceTwinDesiredData.object() != desiredObj) {
            qDebug() << "Reported data changed" << m_deviceTwinDesiredData << desiredObj;
            m_deviceTwinDesiredData.setObject(reportedObj);
            emit q->deviceTwinDesiredDataChanged();
        }

        // Todo: Handle versions.
    }
}

int QAzureIotDevicePrivate::handleDeviceMethodCallback(const char *method_name,
                                                       const unsigned char *payload,
                                                       size_t size,
                                                       unsigned char **response,
                                                       size_t *resp_size)
{
    Q_Q(QAzureIotDevice);

    QString responseStr;

    //TODO: Does QString::fromLatin1 really handle null-signs in the middle of
    //      the string/data buffer?
    int ret = q->handleDeviceMethod(QByteArray(method_name),
                                    QString::fromLatin1((char*)payload, size),
                                    responseStr);

    if (!responseStr.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Response:" << responseStr;
        *response = new unsigned char(responseStr.length());
        memcpy(*response, responseStr.toLatin1().data(), responseStr.length());
        *resp_size = responseStr.length();
    }

    qDebug() << response << resp_size << *response << *resp_size;

    return ret;
}

void QAzureIotDevicePrivate::setDeviceTwinData(QJsonDocument deviceTwinData)
{
    if (m_deviceTwinReportedData != deviceTwinData) {
        QByteArray data = deviceTwinData.toJson(QJsonDocument::Compact);
        qDebug() << data;
        if (IoTHubClient_LL_SendReportedState(m_connection.connectionHandle(),
                                              (const unsigned char*)data.data(),
                                              data.length(),
                                              &deviceTwinReportedStateCallback,
                                              (void *)this) != IOTHUB_CLIENT_OK)
        {
            qDebug() << Q_FUNC_INFO << "Error";
        }
    }
}

int QAzureIotDevicePrivate::handleReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message)
{
    Q_Q(QAzureIotDevice);

    const char* messageId;
    const char* correlationId;
    const char* buffer;
    size_t size;
    MAP_HANDLE mapProperties;
    QAzureIotMessage msg;

    // Message properties
    if ((messageId = IoTHubMessage_GetMessageId(message)) == NULL) {
        messageId = "<null>";
    }
    msg.setMessageId(messageId);

    if ((correlationId = IoTHubMessage_GetCorrelationId(message)) == NULL) {
        correlationId = "<null>";
    }

    if (IoTHubMessage_GetByteArray(message, (const unsigned char**)&buffer, &size) != IOTHUB_MESSAGE_OK) {
        qDebug() << "unable to retrieve the message data";
    } else {
        QByteArray data((char *)buffer, size);
        msg.setMessageData(data);
    }

    mapProperties = IoTHubMessage_Properties(message);
    if (mapProperties != NULL) {
        const char*const* keys;
        const char*const* values;
        size_t propertyCount = 0;
        if (Map_GetInternals(mapProperties, &keys, &values, &propertyCount) == MAP_OK) {
            if (propertyCount > 0) {
                size_t index;

                for (index = 0; index < propertyCount; index++) {
                    msg.addProperty(keys[index], values[index]);
                }
            }
        }
    }

    emit q->newMessage(msg);

    return IOTHUBMESSAGE_ACCEPTED;
}

void QAzureIotDevicePrivate::handleConnectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result,
                                                            IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason)
{
    Q_Q(QAzureIotDevice);
    emit q->connectionStatusChanged(result, reason);
}

QAzureIotDevice::QAzureIotDevice(QByteArray connectionString,
                                 QObject *parent)
    : QObject(parent)
    , d_ptr(new QAzureIotDevicePrivate(this, connectionString))
{
}

QJsonDocument QAzureIotDevice::deviceTwinData()
{
    Q_D(QAzureIotDevice);
    return d->m_deviceTwinRawData;
}

void QAzureIotDevice::setDeviceTwinData(const QJsonDocument document)
{
    Q_D(QAzureIotDevice);
    d->setDeviceTwinData(document);
}

int QAzureIotDevice::handleDeviceMethod(QByteArray methodName, QString payload,
                                        QString &response)
{
    Q_UNUSED(methodName);
    Q_UNUSED(payload);
    Q_UNUSED(response);

    qWarning() << "No methodhandler is implemented. Derive a class from QAzureIotDeviceTemplate<T> to solve this.";

    return -1;
}
