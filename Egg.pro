#-------------------------------------------------
#
# Project created by QtCreator 2017-02-14T15:41:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Egg
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += MainWindow/main.cpp\
        MainWindow/mainwindow.cpp \
    Tree/treenode.cpp \
    Tree/treestate.cpp \
    TreeDisplayWidget/treedisplaywidget.cpp \
    Utilities/Command/ctreestateaddstatement.cpp \
    Utilities/Command/commandinvoker.cpp \
    Utilities/Command/ctreestateaddcut.cpp \
    Utilities/Command/ctreestateselectroot.cpp \
    Utilities/Command/ctreestateselectparent.cpp \
    Utilities/Command/ctreestateselectachild.cpp \
    Utilities/Command/ctreestateselectleft.cpp \
    Utilities/Command/ctreestateselectright.cpp

HEADERS  += MainWindow/mainwindow.h \
    Utilities/constants.h \
    Tree/treenode.h \
    Tree/treestate.h \
    TreeDisplayWidget/treedisplaywidget.h \
    Utilities/Command/icommand.h \
    Utilities/Command/ctreestateaddstatement.h \
    Utilities/Command/commandinvoker.h \
    Utilities/Command/ctreestateaddcut.h \
    Utilities/Command/ctreestateselectroot.h \
    Utilities/Command/ctreestateselectparent.h \
    Utilities/Command/ctreestateselectachild.h \
    Utilities/Command/ctreestateselectleft.h \
    Utilities/Command/ctreestateselectright.h \
    Utilities/Command/allcommands.h

FORMS    += MainWindow/mainwindow.ui \
    TreeDisplayWidget/treedisplaywidget.ui

INCLUDEPATH += $$PWD
