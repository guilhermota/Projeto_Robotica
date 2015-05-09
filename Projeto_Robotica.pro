#-------------------------------------------------
#
# Project created by QtCreator 2015-05-09T15:59:28
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projeto_Robotica
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialport.cpp \
    video.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    serialport.h \
    video.h

FORMS    += mainwindow.ui \
    serialport.ui
