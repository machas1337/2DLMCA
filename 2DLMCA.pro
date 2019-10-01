#-------------------------------------------------
#
# Project created by QtCreator 2017-08-25T17:56:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2DLMCA
TEMPLATE = app


SOURCES += main.cpp\
    decrypter.cpp \
        mainwindow.cpp \
    encrypter.cpp \
    matrix.cpp \
    windowdecryptor.cpp \
    windowoutput.cpp

HEADERS  += mainwindow.h \
    decrypter.h \
    encrypter.h \
    matrix.h \
    windowdecryptor.h \
    windowoutput.h

FORMS    += mainwindow.ui \
    windowdecryptor.ui \
    windowoutput.ui
