#-------------------------------------------------
#
# Project created by QtCreator 2017-03-26T11:08:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = trainer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp
CFLAGS = -l/usr/include/opencv
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann
