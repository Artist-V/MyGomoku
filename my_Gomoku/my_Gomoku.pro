#-------------------------------------------------
#
# Project created by QtCreator 2019-06-21T09:00:16
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = my_Gomoku
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gomokumodel.cpp \
    computerpkgame.cpp \
    onlinepkgame.cpp \
    mysql.cpp \
    mylogin.cpp \
    myregister.cpp \
    userupdate.cpp \
    homewindow.cpp

HEADERS  += mainwindow.h \
    gomokumodel.h \
    computerpkgame.h \
    onlinepkgame.h \
    mysql.h \
    mylogin.h \
    myregister.h \
    userupdate.h \
    homewindow.h

FORMS    += mainwindow.ui \
    computerpkgame.ui \
    onlinepkgame.ui \
    mylogin.ui \
    myregister.ui \
    userupdate.ui \
    homewindow.ui

RESOURCES += \
    res.qrc
