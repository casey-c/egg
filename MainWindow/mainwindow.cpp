#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"
#include "Utilities/Command/allcommands.h"

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

    // Draw the starting node on the text widget
    currentTree->redraw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Key Press */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ICommand* command;

    switch (event->key())
    {
    case Qt::Key_A:
            qDebug() << "A is pressed";
            command = new CTreeStateAddStatement(currentTree,"A");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_Period:
            qDebug() << ". is pressed";
            commandInvoker.repeatLastCommand();
            break;
    case Qt::Key_B:
            qDebug() << "B is pressed";
            command = new CTreeStateAddStatement(currentTree,"B");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_C:
            qDebug() << "C is pressed";
            command = new CTreeStateAddStatement(currentTree,"C");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_D:
            qDebug() << "D is pressed";
            command = new CTreeStateAddStatement(currentTree,"D");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_E:
            qDebug() << "E is pressed";
            command = new CTreeStateAddStatement(currentTree,"E");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_F:
            qDebug() << "F is pressed";
            command = new CTreeStateAddStatement(currentTree,"F");
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_J:
            qDebug() << "J is pressed";
            command = new CTreeStateSelectAChild(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_K:
            qDebug() << "K is pressed";
            command = new CTreeStateSelectParent(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_H:
            qDebug() << "H is pressed";
            command = new CTreeStateSelectLeft(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_L:
            qDebug() << "L is pressed";
            command = new CTreeStateSelectRight(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_Semicolon:
            qDebug() << "; is pressed";
            command = new CTreeStateSelectRoot(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_X:
            qDebug() << "X is pressed";
            command = new CTreeStateAddCut(currentTree);
            commandInvoker.runCommand(command);
            break;
    default:
            QMainWindow::keyPressEvent(event);
    }
}
