#-------------------------------------------------
#
# Project created by QtCreator 2018-07-29T15:22:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer_2
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
        widget.cpp \
    textticker.cpp

HEADERS += \
        widget.h \
    widget.h \
    widget.h \
    widget.h \
    textticker.h

FORMS += \
        widget.ui

QT += multimedia multimediawidgets

RESOURCES += \
    resource/rec.qrc

DISTFILES += \
    resource/5a0b9af401bb4.jpg \
    resource/p1197456005.webp.jpg \
    resource/bigdot.png \
    resource/close_1.png \
    resource/close_2.png \
    resource/cross_15.977464788732px_1153732_easyicon_smaller.net-恢复的.png \
    resource/cross_31.954929577465px_1153732_easyicon.net.png \
    resource/currentItemInLoop.png \
    resource/dot.png \
    resource/hide.png \
    resource/hide_ho.png \
    resource/last.png \
    resource/loop.png \
    resource/M.png \
    resource/next.png \
    resource/pause.png \
    resource/play.png \
    resource/play_ho.png \
    resource/random.png \
    resource/sequenti.png \
    resource/stop.png \
    resource/title.png \
    resource/title_ho.png \
    resource/title_string.png \
    resource/title_string_2.png \
    resource/tray.png \
    resource/valume.png

