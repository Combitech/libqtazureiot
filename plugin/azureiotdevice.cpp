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

#include "azureiotdevice.h"

AzureIotDevice::AzureIotDevice(QObject *parent)
    : QAzureIotDevice(qgetenv("CONNECTION_STRING"), parent)
{
}

int AzureIotDevice::handleDeviceMethod(QByteArray methodName, QString payload, QString &response)
{
    // TODO: Handle response in some way. Hmm

    Q_UNUSED(response);
    emit methodCalled(methodName, payload);
    return 1;
}
