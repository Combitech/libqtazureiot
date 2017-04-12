#-------------------------------------------------
#
# Project created by QtCreator 2017-03-31T16:14:19
#
#-------------------------------------------------

QT       += network
QT       -= gui

INCLUDEPATH += /usr/include/azureiot/inc \
               /usr/include/azureiot

TARGET = qtazureiot
TEMPLATE = lib

DEFINES += LIBQTAZUREIOT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    qazureconnection.cpp \
    qazureiotdevice.cpp \
    qazureiotmessage.cpp

HEADERS +=\
        libqtazureiot_global.h \
    qazureconnection.h \
    qazureiotdevice.h \
    qazureiotdevice_p.h \
    qazureiotmessage.h \
    qazureiotconnection_p.h

LIBS += \
    -L/opt/combitech/ldc/iot/azure-iot-hub/ota-test/build-azure-iot-sdk-c-Desktop-Default/iothub_client \
    -L/opt/combitech/ldc/iot/azure-iot-hub/ota-test/build-azure-iot-sdk-c-Desktop-Default/serializer

LIBS += \
    -liothub_client \
    -liothub_client_mqtt_transport \
    -liothub_client_mqtt_ws_transport \
    -lumqtt \
    -lserializer \
    -laziotsharedutil \
    -lpthread \
    -lcurl \
    -lssl \
    -lcrypto \
    -lm

unix {
    target.path = /usr/lib
    INSTALLS += target
}
