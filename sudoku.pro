#-------------------------------------------------
#
# Project created by QtCreator 2014-10-24T16:03:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sudoku
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp\
        mainwindow.cpp \
    sudokuitem.cpp \
    uisudokuitem.cpp \
    clickablelabel.cpp \
    checkavail.cpp

HEADERS  += mainwindow.h \
    sudokuitem.hpp \
    uisudokuitem.h \
    clickablelabel.h \
    checkavail.hpp

FORMS    += mainwindow.ui \
    uisudokuitem.ui
