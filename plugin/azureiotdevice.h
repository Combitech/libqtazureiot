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

#ifndef AZUREDEVICE_H
#define AZUREDEVICE_H

#include <qazureiotdevice.h>

class AzureIotDevice : public QAzureIotDevice
{
    Q_OBJECT
    Q_DISABLE_COPY(AzureIotDevice)

public:
    AzureIotDevice(QObject *parent = 0);

signals:
    void methodCalled(QString method, QString payload);

private:
    int handleDeviceMethod(QByteArray methodName, QString payload, QString &response);
};

#endif // AZUREDEVICE_H
