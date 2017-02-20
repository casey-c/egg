#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentTree = new TreeState();
    treeDisplayWidget = new TreeDisplayWidget();
    ui->leftLayout->addWidget(treeDisplayWidget);

    QObject::connect(currentTree,
                     SIGNAL(treeChanged(QString)),
                     treeDisplayWidget,
                     SLOT(updateText(QString)));

    //currentTree->addChildStatement("t");
    currentTree->addChildStatement(QString("A"));
    //currentTree->printTree();

}

MainWindow::~MainWindow()
{
    delete ui;
}
