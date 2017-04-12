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

#ifndef QAZUREIOTDEVICE_P_H
#define QAZUREIOTDEVICE_P_H

#include "qazureiotdevice.h"

#include <QJsonDocument>

#include "qazureconnection.h"

class QAzureIotDevicePrivate
{
    Q_DECLARE_PUBLIC(QAzureIotDevice)
public:
    QAzureIotDevicePrivate(QAzureIotDevice *q, QByteArray connectionString);

    void handleDeviceTwinCallback(DEVICE_TWIN_UPDATE_STATE updateState,
                            QByteArray payload);

    int handleDeviceMethodCallback(const char* method_name,
                                    const unsigned char* payload,
                                    size_t size,
                                    unsigned char** response,
                                    size_t* resp_size);
    int handleReceiveMessageCallback(IOTHUB_MESSAGE_HANDLE message);
    void handleConnectionStatusCallback(IOTHUB_CLIENT_CONNECTION_STATUS result,
                                        IOTHUB_CLIENT_CONNECTION_STATUS_REASON reason);
    void setDeviceTwinData(QJsonDocument deviceTwinData);


    //Device Twin info
    QJsonDocument m_deviceTwinRawData;
    QJsonDocument m_deviceTwinReportedData;
    QJsonDocument m_deviceTwinDesiredData;
    int m_deviceTwinReportedVersion;
    int m_deviceTwinDesiredVersion;

private:
    QAzureConnection m_connection;


    QAzureIotDevice *q_ptr;
};

#endif // QAZUREIOTDEVICE_P_H
