#-------------------------------------------------
#
# Project created by QtCreator 2018-02-16T11:09:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ObjectsClassifier
TEMPLATE = app

CONFIG += c++11

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
        mainwindow.cpp \
    external/qcustomplot.cpp \
    obj.cpp \
    datamodel.cpp \
    widgets/colorwidget.cpp \
    attribute.cpp \
    widgets/histogramwidget.cpp \
    widgets/imagesizewidget.cpp

HEADERS += \
        mainwindow.h \
    external/qcustomplot.h \
    obj.h \
    datamodel.h \
    widgets/colorwidget.h \
    const.h \
    attribute.h \
    widgets/histogramwidget.h \
    widgets/imagesizewidget.h

RESOURCES += \
        sources.qrc

win32: RC_FILE = iconsource.rc

FORMS += \
        mainwindow.ui \
    widgets/colorwidget.ui \
    widgets/histogramwidget.ui \
    widgets/imagesizewidget.ui
