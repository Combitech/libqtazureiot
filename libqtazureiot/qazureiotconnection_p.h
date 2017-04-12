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

#ifndef QAZUREIOTCONNECTION_P_H
#define QAZUREIOTCONNECTION_P_H

#include "qazureconnection.h"
#include <iothub_client_ll.h>

class QAzureConnectionPrivate
{
    Q_DECLARE_PUBLIC(QAzureConnection)
public:
    QAzureConnectionPrivate(QAzureConnection *q);

    void doWork();
    bool connectToHub(QString connectionString);
    void handleConnectionStatusCallback(int result, int reason);

private:
    IOTHUB_CLIENT_LL_HANDLE m_iotHubClientHandle;

    QAzureConnection *q_ptr;
};

#endif // QAZUREIOTCONNECTION_P_H
