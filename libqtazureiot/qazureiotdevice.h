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

#ifndef QAZUREIOTDEVICE_H
#define QAZUREIOTDEVICE_H

#include "libqtazureiot_global.h"

#include <QObject>
#include <QMetaMethod>
#include <QJsonDocument>
#include <QDebug>

#include "qazureconnection.h"
#include "qazureiotmessage.h"

class QAzureIotDevicePrivate;
class LIBQTAZUREIOTSHARED_EXPORT QAzureIotDevice : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAzureIotDevice)
    // TODO: Fix this
    //Q_PROPERTY(QJsonDocument deviceTwinData READ deviceTwinData WRITE setDeviceTwinData NOTIFY deviceTwinDataChanged)

public:
    explicit QAzureIotDevice(QByteArray connectionString, QObject *parent = 0);
    QJsonDocument deviceTwinData();

public slots:
    void setDeviceTwinData(const QJsonDocument document);

signals:
    void deviceTwinRawDataChanged();
    void deviceTwinDesiredDataChanged();
    void deviceTwinReportedDataChanged();

    void newMessage(QAzureIotMessage message);
    void connectionStatusChanged(int result, int reason);
private:
    virtual int handleDeviceMethod(QByteArray methodName, QString payload, QString &response);

    QAzureIotDevicePrivate *d_ptr;
};

template <typename T>
class LIBQTAZUREIOTSHARED_EXPORT QAzureIotDeviceTemplate : public QAzureIotDevice
{
public:
    QAzureIotDeviceTemplate(QByteArray connectionString, QObject *parent = 0)
        : QAzureIotDevice(connectionString, parent)
    {
    }

    int handleDeviceMethod(QByteArray methodName, QString payload, QString &response)
    {
        // Check all slots and se if we have a handler
        int ret = -1;
        bool foundMethod = false;
        QByteArray slotName = "on" + methodName;
        const QMetaObject *metaObj = metaObject();
        for (int i=metaObj->methodOffset(); i<metaObj->methodCount(); ++i) {
            QMetaMethod method = metaObj->method(i);
            if (method.methodType() == QMetaMethod::Slot &&
                method.name().toLower() == slotName.toLower())
            {
                // TODO: Add more checks on this method, like parameters
                //       return type and so on.
                foundMethod = true;
                qDebug() << Q_FUNC_INFO << method.name() << slotName;
                metaObj->invokeMethod(this, method.name(),Qt::DirectConnection,
                                      Q_RETURN_ARG(int, ret),
                                      Q_ARG(QString, payload),
                                      Q_ARG(QString&, response));
                qDebug() << Q_FUNC_INFO << "Return value:" << ret;

            }
        }

        if (!foundMethod) {
            qWarning() << "No method handler found for" << methodName;
        }

        return ret;
    }
};


#endif // QAZUREIOTDEVICE_H
