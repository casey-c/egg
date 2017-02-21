#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"
#include <QDebug>

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

    currentTree->addChildCut();
    currentTree->addChildStatement(QString("A"));
    currentTree->selectRoot();
    currentTree->addChildStatement(QString("B"));
    currentTree->selectParent();
    currentTree->addChildDoubleCut();
    currentTree->addChildStatement("C");
    currentTree->selectParent();
    currentTree->selectParent();
    currentTree->addChildStatement("D");
    currentTree->selectRoot();
    currentTree->addChildCut();
    currentTree->addChildCut();
    currentTree->addChildCut();
    currentTree->addChildStatement("E");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Key Press */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_A:
            qDebug() << "A is pressed";
            break;
    case Qt::Key_B:
            qDebug() << "B is pressed";
            break;
    default:
            QMainWindow::keyPressEvent(event);
    }
}
