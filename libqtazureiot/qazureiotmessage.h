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

#ifndef QAZUREIOTMESSAGE_H
#define QAZUREIOTMESSAGE_H

#include <QMetaType>
#include <QByteArray>
#include <QMap>

class QAzureIotMessage
{
    // TODO: Add properties if we would like to use this in QML
    // TODO: We need to add some other properties as well. but we'll do that
    //       later on...
public:
    QAzureIotMessage();
    QAzureIotMessage(const QAzureIotMessage& message);

    QByteArray messageId() const;
    void setMessageId(const QByteArray messageId);

    QByteArray messageData() const;
    void setMessageData(const QByteArray messageData);

    int propertyCount() const;
    QList<QByteArray> propertyKeys() const;
    QByteArray propertyValue(QByteArray property) const;
    void addProperty(const QByteArray property, const QByteArray value);

private:
    QByteArray m_messageId;
    QByteArray m_messageData;
    QMap<QByteArray, QByteArray> m_map;

    friend QDebug operator<< (QDebug d, const QAzureIotMessage &message);
};

Q_DECLARE_METATYPE(QAzureIotMessage)

#endif // QAZUREIOTMESSAGE_H
