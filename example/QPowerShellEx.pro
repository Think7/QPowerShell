#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T13:39:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QPowerShellEx
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp \
    QPowerShellCommand.cpp \
    QPowerShellPipeline.cpp \
    QPowerShell.cpp

HEADERS  += MainWindow.h \
    QPowerShellCommand.h \
    QPowerShellPipeline.h \
    QPowerShell.h

FORMS    += MainWindow.ui
