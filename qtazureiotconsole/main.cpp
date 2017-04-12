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

#include <QCoreApplication>
#include <qazureiotdevice.h>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

#include <mydevice.h>

static QJsonDocument deviceTwinData;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyDevice *device = new MyDevice(qgetenv("CONNECTION_STRING"));
/*    con->setConnectionString(qgetenv("CONNECTION_STRING"));
    qDebug() << "Connect to hub return:" << con->connectToHub();
    qDebug() << con->connected();

    int id = 0;
    con->sendMessage("{\"deviceId\":\"myFirstDevice\",\"windSpeed\":1.2}", id);*/

    device->connect(device, &QAzureIotDevice::deviceTwinRawDataChanged,
                    [=]()
    {
        qDebug() << Q_FUNC_INFO << device->deviceTwinData();
        deviceTwinData = device->deviceTwinData();
    });

    /*QTimer *t = new QTimer();
    t->setInterval(3000);
    t->connect(t, &QTimer::timeout,
               [=] ()
    {
        qDebug() << "Time out" << QDateTime::currentDateTime().toString(Qt::ISODate);
        QJsonObject rootObj;
        rootObj["lastReboot"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        QJsonDocument doc;
        doc.setObject(rootObj);
        device->setDeviceTwinData(doc);
    });
    t->start();*/

    a.exec();
}
