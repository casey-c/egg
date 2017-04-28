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
# You can also select to disable deprecated
#                APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0


SOURCES += MainWindow/main.cpp\
        MainWindow/mainwindow.cpp \
    Tree/treenode.cpp \
    Tree/treestate.cpp \
    TreeDisplayWidget/treedisplaywidget.cpp \
    Utilities/Command/commandinvoker.cpp \
    Utilities/Command/ctreestatesurroundwithcut.cpp \
    Utilities/Command/ctreestatesurroundwithdoublecut.cpp \
    Utilities/Command/itreestateadd.cpp \
    Utilities/Command/itreestateselect.cpp \
    Utilities/Command/itreestatehighlight.cpp \
    Utilities/Command/itreestateremove.cpp \
    Utilities/Command/ctreestatedetachnodebutsavechildren.cpp \
    Utilities/Command/ctreestatesurroundwithcutasgroup.cpp \
    ProofTree/prooftree.cpp \
    ProofTree/prooftreenode.cpp \
    PolishInputWidget/polishinputwidget.cpp \
    PolishInputWidget/PolishTree/polishnode.cpp \
    Grid/gridregion.cpp \
    Grid/grid.cpp \
    Utilities/Command/ctreestateinsertfromformula.cpp \
    Utilities/settingsmanager.cpp \
    Utilities/fileconverter.cpp \
    ProofTree/prooftreeoverview.cpp \
    ProofTree/prooftreenodeview.cpp \
    ProofTree/clickableframe.cpp \
    ProofTree/stepoverview.cpp \
    ProofTree/goalitem.cpp \
    ProofTree/stepitem.cpp \
    ProofTree/hspacer.cpp \
    Utilities/Command/cmodsurroundwithdoublecut.cpp \
    Utilities/Command/imodification.cpp \
    Grid/griddisplayer.cpp \
    Utilities/Command/cmodremovesurroundingdoublecut.cpp \
    Utilities/Command/cmoderasure.cpp \
    Utilities/Command/cmodaddemptydoublecut.cpp

HEADERS  += MainWindow/mainwindow.h \
    Utilities/constants.h \
    Tree/treenode.h \
    Tree/treestate.h \
    TreeDisplayWidget/treedisplaywidget.h \
    Utilities/Command/icommand.h \
    Utilities/Command/ctreestateaddstatement.h \
    Utilities/Command/commandinvoker.h \
    Utilities/Command/ctreestateaddcut.h \
    Utilities/Command/allcommands.h \
    Utilities/Command/ctreestateaddconditionaltemplate.h \
    Utilities/Command/ctreestateaddbiconditionaltemplate.h \
    Utilities/Command/ctreestateaddortemplate.h \
    Utilities/Command/ctreestateadddoublecut.h \
    Utilities/Command/ctreestatesurroundwithcut.h \
    Utilities/Command/ctreestatesurroundwithdoublecut.h \
    Utilities/Command/itreestateadd.h \
    Utilities/Command/itreestateselect.h \
    Utilities/Command/itreestatehighlight.h \
    Utilities/Command/ctreestatehighlightroot.h \
    Utilities/Command/ctreestatehighlightchild.h \
    Utilities/Command/ctreestatehighlightparent.h \
    Utilities/Command/ctreestatehighlightleft.h \
    Utilities/Command/ctreestatehighlightright.h \
    Utilities/Command/ctreestatedeselecthighlighted.h \
    Utilities/Command/ctreestateselecthighlighted.h \
    Utilities/Command/ctreestateclearselection.h \
    Utilities/Command/ctreestateselectchildren.h \
    Utilities/Command/itreestateremove.h \
    Utilities/Command/ctreestatedetachnode.h \
    Utilities/Command/ctreestaterevertselection.h \
    Utilities/Command/ctreestatedetachnodebutsavechildren.h \
    Utilities/Command/ctreestatepounce.h \
    Utilities/Command/ctreestatesurroundwithcutasgroup.h \
    ProofTree/prooftree.h \
    ProofTree/prooftreenode.h \
    PolishInputWidget/polishinputwidget.h \
    PolishInputWidget/PolishTree/polishnode.h \
    Grid/gridregion.h \
    Grid/gridcell.h \
    Grid/grid.h \
    Utilities/Command/ctreestateinsertfromformula.h \
    Utilities/settingsmanager.h \
    Utilities/fileconverter.h \
    ProofTree/prooftreeoverview.h \
    ProofTree/prooftreenodeview.h \
    ProofTree/clickableframe.h \
    ProofTree/stepoverview.h \
    ProofTree/goalitem.h \
    ProofTree/stepitem.h \
    ProofTree/hspacer.h \
    Utilities/Command/cmodsurroundwithdoublecut.h \
    Utilities/Command/imodification.h \
    Grid/griddisplayer.h \
    Utilities/Command/cmodremovesurroundingdoublecut.h \
    Utilities/Command/cmoderasure.h \
    Utilities/Command/cmodaddemptydoublecut.h

FORMS    += MainWindow/mainwindow.ui \
    TreeDisplayWidget/treedisplaywidget.ui \
    PolishInputWidget/polishinputwidget.ui \
    ProofTree/prooftreeoverview.ui \
    ProofTree/prooftreenodeview.ui \
    ProofTree/clickableframe.ui \
    ProofTree/stepoverview.ui \
    Grid/griddisplayer.ui

INCLUDEPATH += $$PWD

# the following files will be included for Unit test build
test {
    message(Test build)

    #QtTest stuff
    QT += testlib

    #Exclude MainWindow/main.cpp to avoid confusion with UnitTest/main.cpp
    SOURCES -= MainWindow/main.cpp

    HEADERS += \
        Test/TestTree.h \
        Test/UnitTest.h

    SOURCES += \
        Test/main.cpp \
        Test/TestTree.cpp \
          Test/UnitTest.cpp

} else {
    #Message for Debug/Release build
    message(Normal build)
}

RESOURCES += \
    resources.qrc

