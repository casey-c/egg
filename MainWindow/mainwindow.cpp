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

  //  currentTree->addChildCut();
  //  currentTree->addChildStatement(QString("A"));
  //  currentTree->selectRoot();
  //  currentTree->addChildStatement(QString("B"));
  //  currentTree->selectParent();
  //  currentTree->addChildDoubleCut();
  //  currentTree->addChildStatement("C");
  //  currentTree->selectParent();
  //  currentTree->selectParent();
  //  currentTree->addChildStatement("D");
  //  currentTree->selectRoot();
  //  currentTree->addChildCut();
  //  currentTree->addChildCut();
  //  currentTree->addChildCut();
  //  currentTree->addChildStatement("E");
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
            currentTree->addChildStatement("A");
            break;
    case Qt::Key_B:
            qDebug() << "B is pressed";
            currentTree->addChildStatement("B");
            break;
    case Qt::Key_C:
            qDebug() << "C is pressed";
            currentTree->addChildStatement("C");
            break;
    case Qt::Key_D:
            qDebug() << "D is pressed";
            currentTree->addChildStatement("D");
            break;
    case Qt::Key_E:
            qDebug() << "E is pressed";
            currentTree->addChildStatement("E");
            break;
    case Qt::Key_F:
            qDebug() << "F is pressed";
            currentTree->addChildStatement("F");
            break;
    case Qt::Key_J:
            qDebug() << "J is pressed";
            currentTree->selectParent();
            break;
    case Qt::Key_K:
            qDebug() << "K is pressed";
            currentTree->selectAChild();
            break;
    case Qt::Key_H:
            qDebug() << "H is pressed";
            currentTree->selectLeftSibling();
            break;
    case Qt::Key_L:
            qDebug() << "L is pressed";
            currentTree->selectRightSibling();
            break;
    case Qt::Key_Semicolon:
            qDebug() << "; is pressed";
            currentTree->selectRoot();
            break;
    case Qt::Key_X:
            qDebug() << "X is pressed";
            currentTree->addChildCut();
            break;
    default:
            QMainWindow::keyPressEvent(event);
    }
}
