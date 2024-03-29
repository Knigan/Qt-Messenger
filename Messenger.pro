QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcontact.cpp \
    changepassword.cpp \
    configurechat.cpp \
    connection.cpp \
    createchat.cpp \
    deletecontact.cpp \
    main.cpp \
    mainwindow.cpp \
    registration.cpp \
    tcpserver.cpp

HEADERS += \
    addcontact.h \
    changepassword.h \
    configurechat.h \
    connection.h \
    createchat.h \
    deletecontact.h \
    mainwindow.h \
    registration.h \
    tcpserver.h

FORMS += \
    addcontact.ui \
    changepassword.ui \
    configurechat.ui \
    connection.ui \
    createchat.ui \
    deletecontact.ui \
    mainwindow.ui \
    registration.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
