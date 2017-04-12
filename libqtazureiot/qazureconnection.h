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

#ifndef QAZURECONNECTION_H
#define QAZURECONNECTION_H

#include <QObject>
#include <QTimer>
#include <QHash>

#include <iothub_message.h>
#include <iothub_client_ll.h>

class QAzureConnection;
struct QAzureConnectionMessageHandle {
    QAzureConnection *connectionObject;
    int messageId;
};

class QAzureConnectionPrivate;
class QAzureConnection : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QAzureConnection)

    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(QString connectionString READ connectionString WRITE setConnectionString NOTIFY connectionStringChanged)

public:
    explicit QAzureConnection(QObject *parent = 0);

    bool connected() const;
    QString connectionString() const;
    IOTHUB_CLIENT_LL_HANDLE connectionHandle();

signals:
    void connectedChanged();
    void connectionStringChanged();
    void connectionStatusChanged(int result, int reason);

public slots:
    bool connectToHub();
    void setConnectionString(const QString connectionString);

private slots:
    void onTimeout();

private:
    QTimer m_timer;
    QString m_connectionString;

    QAzureConnectionPrivate *d_ptr;
};

#endif // QAZURECONNECTION_H
