QT       += core gui printsupport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chanform.cpp \
    chminiform.cpp \
    connectionform.cpp \
    main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    settingsform.cpp

HEADERS += \
    DeviceConstants.h \
    chanform.h \
    chminiform.h \
    connectionform.h \
    mainwindow.h \
    qcustomplot.h \
    settingsform.h

FORMS += \
    chanform.ui \
    chminiform.ui \
    connectionform.ui \
    mainwindow.ui \
    settingsform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
