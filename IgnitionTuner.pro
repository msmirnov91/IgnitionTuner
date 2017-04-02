#-------------------------------------------------
#
# Project created by QtCreator 2017-02-01T23:11:37
#
#-------------------------------------------------

QT       += core gui

CONFIG += qwt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IgnitionTuner
TEMPLATE = app


SOURCES += main.cpp\
    model.cpp \
    presenter.cpp \
    view.cpp \
    abstractstoragestrategy.cpp \
    testdatastoragestrategy.cpp \
    firstversionstrategy.cpp \
    intelhex.cpp

HEADERS  += \
    model.h \
    presenter.h \
    view.h \
    abstractstoragestrategy.h \
    testdatastoragestrategy.h \
    firstversionstrategy.h \
    intelhex.h

FORMS    += view.ui

