#-------------------------------------------------
#
# Project created by QtCreator 2015-05-09T15:59:28
#
#-------------------------------------------------

QT += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projeto_Robotica
TEMPLATE = app

INCLUDEPATH += C:\OpenCV\opencv\build\include

LIBS += -LC:\OpenCV\opencv\build\x86\vc12\lib \
    opencv_core2410.lib \
    opencv_highgui2410.lib \
    opencv_imgproc2410.lib \
    opencv_objdetect2410.lib \

SOURCES += main.cpp\
        mainwindow.cpp \
    serialport.cpp \
    video.cpp \
    qtopencv.cpp \
    facedetector.cpp \
    medidorperformance.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    serialport.h \
    video.h \
    qtopencv.h \
    facedetector.h \
    medidorperformance.h

FORMS    += mainwindow.ui \
    serialport.ui

RESOURCES += \
    resources.qrc
