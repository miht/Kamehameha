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

INCLUDEPATH += "/Applications/Autodesk/FBX SDK/2018.0/include"
#LIBS += -L"/Applications/Autodesk/FBX SDK/2015.1/lib"
#LIBS += "/Applications/Autodesk/FBX SDK/2015.1/lib/clang/release/libfbxsdk.a"
#LIBS +=

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
        src/main.cpp \
    src/vector3d.cpp \
    src/vertex3d.cpp \
    src/objparser.cpp \
    src/material.cpp \
    src/ray.cpp \
    src/triangle.cpp \
    src/kamehameha.cpp \
    src/raytracer.cpp \
    src/color.cpp \
    src/light.cpp \
    src/intersection.cpp \
    src/wireframer.cpp \
    src/scene.cpp \
    src/model.cpp \
    src/renderer.cpp \
    src/face.cpp \
    src/rasterizer.cpp \
    src/object.cpp \
    src/boundingbox.cpp \
    src/illuminationmodel.cpp \
    src/pathtracer.cpp \
    src/camera.cpp \
    src/mathops.cpp \
    src/tracer.cpp \
    src/kdnode.cpp \
    src/geometry.cpp \
    src/transform.cpp \
    src/settings.cpp \
    src/fbxparser.cpp \
    src/aboutdialog.cpp \
    src/renderview.cpp \
    src/scenemetadata.cpp

HEADERS += \
    src/vertex3d.h \
    src/objparser.h \
    src/material.h \
    src/triangle.h \
    src/kamehameha.h \
    src/raytracer.h \
    src/color.h \
    src/light.h \
    src/intersection.h \
    src/wireframer.h \
    src/scene.h \
    src/model.h \
    src/renderer.h \
    src/face.h \
    src/rasterizer.h \
    src/object.h \
    src/boundingbox.h \
    src/illuminationmodel.h \
    src/pathtracer.h \
    src/camera.h \
    src/mathops.h \
    src/tracer.h \
    src/kdnode.h \
    src/geometry.h \
    src/transform.h \
    src/settings.h \
    src/fbxparser.h \
    src/aboutdialog.h \
    src/renderview.h \
    src/scenemetadata.h

FORMS += \
        mainwindow.ui \
    outputdialog.ui

DISTFILES += \
    ../ProjectTest/cube.obj


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/release/' -lfbxsdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/debug/' -lfbxsdk
else:unix: LIBS += -L$$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/' -lfbxsdk

INCLUDEPATH += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release'
DEPENDPATH += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release'

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/release/libfbxsdk.a'
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/debug/libfbxsdk.a'
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/release/fbxsdk.lib'
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/debug/fbxsdk.lib'
else:unix: PRE_TARGETDEPS += $$PWD/'../../../../Applications/Autodesk/FBX SDK/2018.0/lib/clang/release/libfbxsdk.a'

RESOURCES += \
    images.qrc \
    fonts.qrc
