#-------------------------------------------------
#
# Project created by QtCreator 2015-05-09T15:59:28
#
#-------------------------------------------------

QT += core gui
QT += serialport
QT += sql
QT += network
QTPLUGIN += QSQLMYSQL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projeto_Robotica
TEMPLATE = app

INCLUDEPATH += C:\OpenCV\opencv\build\include
#INCLUDEPATH += C:\Programs\opencv24\opencv\build\include

#LIBS += -LC:\Programs\opencv24\opencv\build\x86\vc12\lib \
#    opencv_core249.lib \
#    opencv_highgui249.lib \
#    opencv_imgproc249.lib \
#    opencv_objdetect249.lib \
#    opencv_nonfree249.lib \
#    opencv_video249.lib \
#    opencv_contrib249.lib \
#    opencv_features2d249.lib \
#    opencv_calib3d249.lib \
#    opencv_flann249.lib \

LIBS += -LC:\OpenCV\opencv\build\x86\vc12\lib \
    opencv_core2410.lib \
    opencv_highgui2410.lib \
    opencv_imgproc2410.lib \
    opencv_objdetect2410.lib \
    opencv_nonfree2410.lib \
    opencv_video2410.lib \
    opencv_contrib2410.lib \
    opencv_features2d2410.lib \
    opencv_calib3d2410.lib \
    opencv_flann2410.lib \

SOURCES += main.cpp\
    mainwindow.cpp \
    serialport.cpp \
    video.cpp \
    qtopencv.cpp \
    facedetector.cpp \
    medidorperformance.cpp \
    facetracker.cpp \
    database.cpp \
    sobre.cpp \
    filedownloader.cpp \
    recognizer.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    serialport.h \
    video.h \
    qtopencv.h \
    facedetector.h \
    medidorperformance.h \
    facetracker.h \
    database.h \
    sobre.h \
    filedownloader.h \
    recognizer.h

FORMS    += mainwindow.ui \
    serialport.ui \
    sobre.ui

RESOURCES += \
    resources.qrc
