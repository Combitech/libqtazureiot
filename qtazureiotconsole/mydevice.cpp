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

#include "mydevice.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

MyDevice::MyDevice(QByteArray connectionString, QObject *parent)
    : QAzureIotDeviceTemplate(connectionString, parent)
{
    connect(this, &MyDevice::newMessage,
            this, &MyDevice::onNewMessage);

    connect(this, &MyDevice::connectionStatusChanged,
            this, &MyDevice::onConnectionStatusChanged);
}

int MyDevice::onReboot(QString payload, QString &response)
{
    qDebug() << Q_FUNC_INFO << payload;
    QJsonDocument doc;
    QJsonObject obj;
    obj["return"] = "Testing";
    doc.setObject(obj);
    qDebug() << doc.toJson(QJsonDocument::Compact);
    response = doc.toJson(QJsonDocument::Compact);
    return 1;
}

void MyDevice::onNewMessage(QAzureIotMessage msg)
{
    qDebug() << Q_FUNC_INFO << msg;
}

void MyDevice::onConnectionStatusChanged(int result, int reason)
{
    qDebug() << Q_FUNC_INFO << result << reason;
}
