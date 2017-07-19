#-------------------------------------------------
#
# Project created by QtCreator 2017-07-16T16:14:20
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notepad
TEMPLATE = app

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
        main.cpp \
        mainwindow.cpp \
    gotodialog.cpp \
    finddialog.cpp \
    replacedialog.cpp \
    aboutdialog.cpp

HEADERS += \
        mainwindow.h \
    gotodialog.h \
    finddialog.h \
    replacedialog.h \
    aboutdialog.h

FORMS += \
        mainwindow.ui \
    gotodialog.ui \
    finddialog.ui \
    replacedialog.ui \
    aboutdialog.ui

RC_FILE = app.rc

RESOURCES += \
    images.qrc \
    qm.qrc
