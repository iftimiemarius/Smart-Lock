#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T19:40:16
#
#-------------------------------------------------

QT       += core
QT += network
QT       -= gui

TARGET = server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    myserver.cpp

HEADERS += \
    myserver.h
CFLAGS = -l/usr/include/opencv
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann

LIBS += -L/usr/local/lib \
-lwiringPi
