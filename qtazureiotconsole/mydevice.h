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

#ifndef MYDEVICE_H
#define MYDEVICE_H

#include <QObject>
#include <qazureiotdevice.h>
#include <qazureiotmessage.h>
class MyDevice : public QAzureIotDeviceTemplate<MyDevice>
{
    Q_OBJECT
public:
    MyDevice(QByteArray connectionString, QObject *parent = 0);

public slots:
    int onReboot(QString payload, QString &response);
    void onNewMessage(QAzureIotMessage msg);
    void onConnectionStatusChanged(int result, int reason);

};

#endif // MYDEVICE_H
