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

#include "qazureiotmessage.h"
#include <QDebug>

QAzureIotMessage::QAzureIotMessage()
{
}

QAzureIotMessage::QAzureIotMessage(const QAzureIotMessage& message)
{
    m_messageId = message.m_messageId;
    m_messageData = message.m_messageData;
    m_map = message.m_map;
}

QByteArray QAzureIotMessage::messageId() const
{
    return m_messageId;
}

void QAzureIotMessage::setMessageId(const QByteArray messageId)
{
    m_messageId = messageId;
}

QByteArray QAzureIotMessage::messageData() const
{
    return m_messageData;
}

void QAzureIotMessage::setMessageData(const QByteArray messageData)
{
    m_messageData = messageData;
}

int QAzureIotMessage::propertyCount() const
{
    return m_map.count();
}

QList<QByteArray> QAzureIotMessage::propertyKeys() const
{
    return m_map.keys();
}

QByteArray QAzureIotMessage::propertyValue(QByteArray property) const
{
    if (m_map.contains(property)) {
        return m_map.value(property);
    }
    return QByteArray();
}

void QAzureIotMessage::addProperty(const QByteArray property, const QByteArray value)
{
    m_map[property] = value;
}

QDebug operator<<(QDebug debug, const QAzureIotMessage &m)
{
    //QDebugStateSaver saver(debug);
    debug.nospace() << "QAzureIotMessage( msgId:"
                    << m.messageId() << ", data:" << m.messageData()
                    << ", properties: " << m.m_map << ")";
    return debug;
}
