QT       += core gui network sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    delfriend.cpp \
    localdb.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    registerdialog.cpp \
    widget.cpp\
    clientfile.cpp\
    blacklist.cpp\
    changename.cpp\
    changepassword.cpp\
    changepic.cpp\
    filetransfer.cpp\
    mylabel.cpp\
    settingdialog.cpp\
    changephone.cpp

HEADERS += \
    addfriend.h \
    delfriend.h \
    localdb.h \
    logindialog.h \
    mainwindow.h \
    registerdialog.h \
    widget.h\
    clientfile.h\
    blacklist.h\
    changename.h\
    changepassword.h\
    changepic.h\
    filetransfer.h\
    mylabel.h\
    settingdialog.h\
    changephone.h

FORMS += \
    addfriend.ui \
    delfriend.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui\
    widget.ui\
    clientfile.ui\
    blacklist.ui\
    changename.ui\
    changepassword.ui\
    changepic.ui\
    filetransfer.ui\
    settingdialog.ui\
    changephone.ui

RESOURCES += \
    images.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


