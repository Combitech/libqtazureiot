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

#include "qazureconnection.h"
#include "qazureiotconnection_p.h"

#include <iothubtransportmqtt.h>
#include <iothubtransportmqtt_websockets.h>
#include <azure_c_shared_utility/threadapi.h>
#include <azure_c_shared_utility/platform.h>

#include <QTime>
#include <QDebug>

static void connectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result, IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason, void* userContextCallback)
{
    QAzureConnectionPrivate *obj = (QAzureConnectionPrivate*)userContextCallback;
    if (obj) {
        obj->handleConnectionStatusCallback(result, reason);
    }
}

QAzureConnectionPrivate::QAzureConnectionPrivate(QAzureConnection *q)
    : m_iotHubClientHandle(NULL)
    , q_ptr(q)
{
}

bool QAzureConnectionPrivate::connectToHub(QString connectionString)
{
    m_iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString.toLatin1().data(),
                                                                      MQTT_WebSocket_Protocol);
    if (!m_iotHubClientHandle) {
        qDebug() << "ERROR: iotHubClientHandle is NULL!";
        return false;
    }

    // Enable debug trace.
    // TODO Add a property if this should be enabled or not
    bool traceOn = true;
    IoTHubClient_LL_SetOption(m_iotHubClientHandle, "logtrace", &traceOn);

    // Register callbacks
    if (IoTHubClient_LL_SetConnectionStatusCallback(m_iotHubClientHandle,
                                                    &connectionStatusCallback,
                                                    (void *)this) != IOTHUB_CLIENT_OK)
    {
        qDebug() << "ERROR: IoTHubClient_LL_SetConnectionStatusCallback failed";
    }

    return true;
}

void QAzureConnectionPrivate::doWork()
{
    IoTHubClient_LL_DoWork(m_iotHubClientHandle);
}

void QAzureConnectionPrivate::handleConnectionStatusCallback(int result, int reason)
{
    Q_Q(QAzureConnection);
    emit q->connectionStatusChanged(result, reason);
}

QAzureConnection::QAzureConnection(QObject *parent)
    : QObject(parent)
    , d_ptr(new QAzureConnectionPrivate(this))
{
    qsrand(QTime::currentTime().msec());
    connect(&m_timer, &QTimer::timeout,
            this, &QAzureConnection::onTimeout);

    if (platform_init() != 0) {
        qDebug() << "Error: platform_init() failed";
    }
}

bool QAzureConnection::connectToHub()
{
    Q_D(QAzureConnection);

    if (d->connectToHub(m_connectionString)) {
        m_timer.setInterval(50);
        m_timer.start();

        return true;
    }
    return false;
}

IOTHUB_CLIENT_LL_HANDLE QAzureConnection::connectionHandle()
{
    Q_D(QAzureConnection);
    return d->m_iotHubClientHandle;
}

bool QAzureConnection::connected() const
{
    return false;
}

QString QAzureConnection::connectionString() const
{
    return m_connectionString;
}

void QAzureConnection::setConnectionString(const QString connectionString)
{
    if (m_connectionString != connectionString) {
        if (connected()) {
            // Disconnect
        }
        m_connectionString = connectionString;
        qDebug() << Q_FUNC_INFO << m_connectionString;
        emit connectionStringChanged();
    }
}

void QAzureConnection::onTimeout()
{
    Q_D(QAzureConnection);
    d->doWork();
}
