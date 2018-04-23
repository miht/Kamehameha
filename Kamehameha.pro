#-------------------------------------------------
#
# Project created by QtCreator 2018-02-11T01:41:13
#
#-------------------------------------------------

QT       += core gui concurrent
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
    vertex3d.cpp \
    objparser.cpp \
    material.cpp \
    ray.cpp \
    triangle.cpp \
    kamehameha.cpp \
    raytracer.cpp \
    color.cpp \
    light.cpp \
    intersection.cpp \
    wireframer.cpp \
    scene.cpp \
    model.cpp \
    renderer.cpp \
    face.cpp \
    rasterizer.cpp \
    object.cpp \
    boundingbox.cpp \
    illuminationmodel.cpp \
    pathtracer.cpp \
    camera.cpp \
    mathops.cpp \
    tracer.cpp \
    kdnode.cpp \
    geometry.cpp \
    transform.cpp

HEADERS += \
    vertex3d.h \
    objparser.h \
    material.h \
    triangle.h \
    kamehameha.h \
    raytracer.h \
    color.h \
    light.h \
    intersection.h \
    wireframer.h \
    scene.h \
    model.h \
    renderer.h \
    face.h \
    rasterizer.h \
    object.h \
    boundingbox.h \
    illuminationmodel.h \
    pathtracer.h \
    camera.h \
    mathops.h \
    tracer.h \
    kdnode.h \
    geometry.h \
    transform.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    ../ProjectTest/cube.obj

