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

import QtQuick 2.6
import QtQuick.Window 2.2
import com.azureiot 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World Azure IoT Hub")

    AzureIotDevice {

    }
}
