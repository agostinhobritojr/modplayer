#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T09:52:16
#
#-------------------------------------------------

QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = player
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    spectrograph.cpp \
    fft.cpp \
    controls.cpp \
    fftcalc.cpp \
    mediainfo.cpp \
    medialibrary.cpp \
    playlistmodel.cpp
 
HEADERS  += mainwindow.h \
    spectrograph.h \
    fft.h \
    controls.h \
    fftcalc.h \
    mediainfo.h \
    abstractcontrol.h \
    abstractspectrograph.h \
    abstractmediainfo.h \
    medialibrary.h \
    playlistmodel.h
   fft.h

FORMS    += mainwindow.ui \
    controls.ui \
    mediainfo.ui \
    medialibrary.ui

RESOURCES += \
    resources.qrc
