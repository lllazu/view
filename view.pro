QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = view
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
    highlighter.cpp

HEADERS  += mainwindow.h \
    highlighter.h

OTHER_FILES = README.md
