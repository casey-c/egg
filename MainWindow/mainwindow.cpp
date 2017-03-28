#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"
#include "Utilities/Command/allcommands.h"

#include "PolishInputWidget/polishinputwidget.h"

#include "Grid/grid.h"

#include "Utilities/fileconverter.h"

#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    copiedTree(NULL),
    keybindMode(constants::MODE_DEFAULT)
{
    ui->setupUi(this);

    currentTree = new TreeState();
    treeDisplayWidget = new TreeDisplayWidget();
    ui->leftLayout->addWidget(treeDisplayWidget);

    // Tell the widget to redraw when the tree updates
    QObject::connect(currentTree,
                     SIGNAL(treeChanged(QString)),
                     treeDisplayWidget,
                     SLOT(updateText(QString)));

    // Update the undo/redo menu bar when the commandInvoker updates
    QObject::connect(&commandInvoker,
                     SIGNAL(updateMenu(QString,QString,QString,
                                       bool,bool,bool)),
                     this,
                     SLOT(updateUndoMenu(QString,QString,QString,
                                         bool,bool,bool)));

    // Give functionality to the undo/redo menu options
    QObject::connect(ui->actionUndo,
                     &QAction::triggered,
                     [this] {
                         this->commandInvoker.undoLastCommand();
                     });
    QObject::connect(ui->actionRedo,
                     &QAction::triggered,
                     [this] {
                         this->commandInvoker.redoLastCommand();
                     });
    QObject::connect(ui->actionRepeat,
                     &QAction::triggered,
                     [this] {
                         this->commandInvoker.repeatLastCommand();
                     });

    // Draw the starting node on the text widget
    currentTree->redraw();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete treeDisplayWidget;
    delete currentTree;
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
    qDebug() << "Key pressed: " << event->text();
    ICommand* command;

    // A-F will add that letter as a statement
    if (event->key() >= 65 && event->key() <= 70)
    {
        qDebug() << "Pressed A-F";
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
        command = new CTreeStateHighlightChild(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_K:
        qDebug() << "K is pressed";
        command = new CTreeStateHighlightParent(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_H:
        qDebug() << "H is pressed";
        command = new CTreeStateHighlightLeft(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_I:
    {
        qDebug() << "I is pressed";
        PolishInputWidget* widget = new PolishInputWidget();

        // Connect it up
        connect( widget,
                 SIGNAL(sendCompletedFormula(TreeNode*)),
                 this,
                 SLOT(insertFromFormula(TreeNode*)));

        widget->show();
        break;
    }
    case Qt::Key_L:
        qDebug() << "L is pressed";
        command = new CTreeStateHighlightRight(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_M:
        qDebug() << "M is pressed";
        command = new CTreeStateDetachNodeButSaveChildren(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_N:
        qDebug() << "N is pressed";
        command = new CTreeStateDetachNode(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_O:
        qDebug() << "O is pressed";
        if (QString::compare(event->text(), "O"))
            command = new CTreeStateSurroundWithCutAsGroup(currentTree);
        else
            command = new CTreeStateSurroundWithCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_P: // TODO: change this to a group maybe
        qDebug() << "P is pressed";
        command = new CTreeStateSurroundWithDoubleCut(currentTree);
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
    case Qt::Key_S:
        qDebug() << "S is pressed.";
        keybindMode = constants::MODE_SELECT; // Enter select mode
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
    case Qt::Key_Y:
        qDebug() << "Y is pressed";
        commandInvoker.redoLastCommand();
        break;
    case Qt::Key_Z:
        qDebug() << "Z is pressed";
        command = new CTreeStateAddDoubleCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_1: // DEBUG: copy the current tree
    {
        qDebug() << "1 is pressed";
        if (copiedTree != NULL)
            delete copiedTree;

        copiedTree = new TreeState(currentTree);
        break;
    }
    case Qt::Key_2: // DEBUG: swap between copied tree and current tree
    {
        qDebug() << "2 is pressed";

        // Disconnect the old tree
        currentTree->disconnect();

        // Swap connections
        TreeState* temp = currentTree;
        currentTree = copiedTree;
        copiedTree = temp;

        // Reconnect the proper tree
        QObject::connect(currentTree,
                         SIGNAL(treeChanged(QString)),
                         treeDisplayWidget,
                         SLOT(updateText(QString)));

        currentTree->redraw();
        break;
    }
    case Qt::Key_4:
        qDebug() << "DEBUG ONLY";
        currentTree->addPlaceholderToHighlighted();
        currentTree->redraw();
        break;
    case Qt::Key_9:
    {
        qDebug() << "9 is pressed";
        Grid g(currentTree);

        ui->tempGridText->setText(g.toPlaintext());
        break;
    }
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
        command = new CTreeStateHighlightRoot(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_BracketLeft:
        qDebug() << "[ is pressed";
        keybindMode = constants::MODE_POUNCE;
        currentTree->setPounceIDs();
        currentTree->drawPounceTree();
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
 * Handle a key press in Select mode.
 *
 * Select mode gives alternate controls to the W,A,X, and D keys
 *
 * Controls:
 *      A: selects all children of the highlighted node
 *      C: clears the selection
 *      D: deselects the highlighted node
 *      F: selects the highlighted node
 *      S: exits select mode back to default mode
 */
void MainWindow::handleKeyPressSelect(QKeyEvent *event)
{
    ICommand* command;

    switch (event->key())
    {
    case Qt::Key_A:
        command = new CTreeStateSelectChildren(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_C:
        command = new CTreeStateClearSelection(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_D:
        command = new CTreeStateDeselectHighlighted(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_F:
        command = new CTreeStateSelectHighlighted(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_R:
        command = new CTreeStateRevertSelection(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_S:
        keybindMode = constants::MODE_DEFAULT;
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

}

/*
 * Pounce mode:
 *
 * After pressing [ for the first time, the application enters pounce mode.
 * This redraws the tree with two letter identifiers enabling rapid movement.
 *
 * In pounce mode, the first letter typed is saved in the pounce target. After
 * entering a second letter, the total string is sent to the tree to see if it
 * can highlight the node with that particular pounce string.
 *
 * The app reverts back to default mode after the second key press, regardless
 * of whether or not the pounce was successful.
 *
 * As of right now, pouncing is limited to a total of 32 possible nodes: these
 * are obtained through combinations of ASDF and JKL; keys, one from each set.
 * I chose these keys because they are the default positions for the fingers,
 * and I made pounce work by selecting one letter from each hand. I can extend
 * this easily by adding any other letters (G and H are the most likely
 * additions) or removing the restriction that one bit must come from each hand.
 *
 * This choice was mostly personal preference: I find it easy to type AJ - a
 * string that takes one key from each hand - as opposed to AA, which seems a
 * little bit slower.
 */
void MainWindow::handleKeyPressPounce(QKeyEvent *event)
{
    QChar letter = (event->text())[0];

    // Make sure the event is ok
    switch (event->key())
    {
        case Qt::Key_A: break;
        case Qt::Key_S: break;
        case Qt::Key_D: break;
        case Qt::Key_F: break;
        case Qt::Key_J: break;
        case Qt::Key_K: break;
        case Qt::Key_L: break;
        case Qt::Key_Semicolon: break;

        default: return; // Invalid
    }

    qDebug() << "Typed " << letter;

    if (!typedOne)
    {
        qDebug() << "This was the first key press";

        // Reset the target and set the first key
        pounceTarget = "";
        pounceTarget += letter;

        typedOne = true;
    }
    else
    {
        qDebug() << "This was the second key press";
        typedOne = false;

        pounceTarget += letter;
        pounceTarget = pounceTarget.toUpper();

        // Pounce to the string, if it exists
        qDebug() << "Pouncing to " << pounceTarget;
        ICommand* command = new CTreeStatePounce(currentTree, pounceTarget);
        commandInvoker.runCommand(command);

        // Reset the mode
        keybindMode = constants::MODE_DEFAULT;
        currentTree->redraw();
    }



}

/*
 * All key presses go here first. The keybindMode will turn the event over to
 * the proper function for further processing.
 */
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed in mode " << keybindMode;

    // Handle key press based on mode
    switch (keybindMode)
    {
    case constants::MODE_DEFAULT:
        handleKeyPressDefault(event);
        break;
    case constants::MODE_Q:
        handleKeyPressQ(event);
        break;
    case constants::MODE_SELECT:
        handleKeyPressSelect(event);
        break;
    case constants::MODE_POUNCE:
        handleKeyPressPounce(event);
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

}


/*
 * Update the menu bar text as needed
 */
void MainWindow::updateUndoMenu(QString undo, QString redo, QString repeat,
                                bool undoable, bool redoable, bool repeatable)
{
    qDebug() << "Updating undo menu text";

    ui->actionUndo->setEnabled(undoable);
    ui->actionUndo->setText(undo);

    ui->actionRedo->setEnabled(redoable);
    ui->actionRedo->setText(redo);

    ui->actionRepeat->setEnabled(repeatable);
    ui->actionRepeat->setText(repeat);
}

void MainWindow::insertFromFormula(TreeNode *root)
{
    ICommand* command = new CTreeStateInsertFromFormula(currentTree, root);
    commandInvoker.runCommand(command);
}

void MainWindow::on_actionOpen_triggered()
{
    // Set up the dialog box
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter("EGG files (*.egg)");
    dialog.setViewMode(QFileDialog::Detail);

    // Activate the dialog box
    if (dialog.exec())
    {
        QList<QUrl> urls = dialog.selectedUrls();
        QUrl url = urls.first();

        // Attempt to convert into a MainWindow object
        MainWindow* newWindow = FileConverter::load(url);

        if (newWindow != nullptr)
            newWindow->show();
    }

}
