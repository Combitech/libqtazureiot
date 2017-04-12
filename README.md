# libqtazureiot
libqtazureiot is a library that handles all connections to Azure's IoT hub and makes it Qt-ich. All communication with the library is done using signals & slots which is the Qt way. libazureiot allows you connect your device to the cloud in a simple and easy way. You can event run this in QML.

## The goal for verion 0.1
This project is still in its very early stage and there are still code that needs to be written.

The goal for version 0.1 is to have a working library both in C++ and QML that can:
* Handle cloud-to-device messages.
* Handle device-to-cloud messages,
* Handle device-twin data.
* Handle cloude method calling.
* Allow connection managment.
* And some more stuff.

## azureiot SDK
We heavely realy on Microsofts SDKs and the Linux-platform. This is still only tested in Linux, but should work well on other OS. In the long run it would be great to implement a Qt-platform in AzureIoT SDK. But first things first.

