#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"
#include "Utilities/Command/allcommands.h"

#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    keybindMode(constants::MODE_DEFAULT)
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

/* End timer slot */
void MainWindow::endTimer()
{
    qDebug() << "Timer finished, reverting to default mode";
    keybindMode = constants::MODE_DEFAULT;
}

/*
 * Handle a key press in default mode.
 *
 * This function will perform the default actions tied to each keybind:
 *
 *      Modify
 *      A-F:    adds statement with that letter
 *      R:      adds implication/conditional template
 *      T:      adds biconditional template
 *      V:      adds or template
 *      X:      adds cut
 *      Z:      adds double cut
 *
 *      Change mode
 *      Q:      enters Q mode (press second key to add that letter to tree)
 */
void MainWindow::handleKeyPressDefault(QKeyEvent *event)
{
    ICommand* command;

    // A-F will add that letter as a statement
    if (event->key() >= 65 && event->key() <= 70)
    {
        command = new CTreeStateAddStatement(currentTree,
                                             event->text().at(0).toUpper());
        commandInvoker.runCommand(command);
        return;
    }

    // Other keys will have more specific functions
    switch (event->key())
    {
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
    case Qt::Key_Q:
            qDebug() << "Q is pressed";
            keybindMode = constants::MODE_Q;
            QTimer::singleShot(1000,this,SLOT(endTimer()));
            break;
    case Qt::Key_R:
            qDebug() << "R is pressed.";
            command = new CTreeStateAddConditionalTemplate(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_T:
            qDebug() << "T is pressed";
            command = new CTreeStateAddBiconditionalTemplate(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_U:
            qDebug() << "U is pressed";
            commandInvoker.undoLastCommand();
            break;
    case Qt::Key_V:
            qDebug() << "V is pressed";
            command = new CTreeStateAddOrTemplate(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_X:
            qDebug() << "X is pressed";
            command = new CTreeStateAddCut(currentTree);
            commandInvoker.runCommand(command);
            break;
    case Qt::Key_0:
            qDebug() << "0 is pressed";
            delete(currentTree);
            currentTree = new TreeState();
            QObject::connect(currentTree,
                             SIGNAL(treeChanged(QString)),
                             treeDisplayWidget,
                             SLOT(updateText(QString)));
            currentTree->redraw();
            break;
    case Qt::Key_Period:
            qDebug() << ". is pressed";
            commandInvoker.repeatLastCommand();
            break;
    case Qt::Key_Semicolon:
            qDebug() << "; is pressed";
            command = new CTreeStateSelectRoot(currentTree);
            commandInvoker.runCommand(command);
            break;
    default:
            QMainWindow::keyPressEvent(event);
    }

}

/*
 * Handle a key press in Q mode.
 *
 * Q mode will add a statement with a label corresponding to the next A-Z key
 * pressed. After hitting any key, the mode reverts to default
 */
void MainWindow::handleKeyPressQ(QKeyEvent *event)
{
    // Revert the mode
    keybindMode = constants::MODE_DEFAULT;

    // Check A-Z
    QString string = event->text();
    if (string.length() == 1 && string.at(0).isLetter())
    {
        ICommand* command = new CTreeStateAddStatement(currentTree,
                                                       string.toUpper());
        commandInvoker.runCommand(command);
        return;
    }
    else
    {
        qDebug() << "Invalid key press";
        QMainWindow::keyPressEvent(event);
        return;
    }

}

/*
 * All key presses go here first. The keybindMode will turn the event over to
 * the proper function for further processing.
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Handle key press based on mode
    switch (keybindMode)
    {
    case constants::MODE_DEFAULT:
        handleKeyPressDefault(event);
        break;
    case constants::MODE_Q:
        handleKeyPressQ(event);
        break;
    }

}
