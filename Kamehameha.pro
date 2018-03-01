#-------------------------------------------------
#
# Project created by QtCreator 2018-02-11T01:41:13
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kamehameha
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    vector3d.cpp \
    model3d.cpp \
    vertex3d.cpp \
    objparser.cpp \
    vector2d.cpp \
    material.cpp \
    ray.cpp \
    triangle.cpp \
    kamehameha.cpp \
    raytracer.cpp \
    color.cpp

HEADERS += \
    vector3d.h \
    model3d.h \
    vertex3d.h \
    objparser.h \
    vector2d.h \
    material.h \
    ray.h \
    triangle.h \
    kamehameha.h \
    raytracer.h \
    color.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    ../ProjectTest/cube.obj

