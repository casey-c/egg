#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"
#include "Utilities/Command/allcommands.h"

#include "PolishInputWidget/polishinputwidget.h"

#include "Grid/grid.h"

#include "Utilities/fileconverter.h"

//#include "ProofTree/prooftreeoverview.h"

#include "ProofTree/stepoverview.h"

#include <QDebug>
#include <QTimer>
#include <QFileDialog>
#include <QUrl>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    copiedTree(NULL),
    keybindMode(constants::MODE_DEFAULT)
{
    ui->setupUi(this);

    /* Set up the tree displayer */
    treeDisplayWidget = new TreeDisplayWidget();
    ui->detailsPlaceholderLayout->addWidget(treeDisplayWidget);

    /* Setup the grid displayer */
    gridDisplayer = new GridDisplayer(this);
    ui->gridPlaceholderLayout->addWidget( gridDisplayer );

    // Give actions shortcuts
    ui->actionNew->setShortcut(QKeySequence::New);
    ui->actionOpen->setShortcut(QKeySequence::Open);
    ui->actionSave->setShortcut(QKeySequence::Save);

    /* The current tree ( premise graph ) */
    currentTree = new TreeState();

    // Tell the widget to redraw when the tree updates
    connectTree();

    //QObject::connect(currentTree,
                     //SIGNAL(treeChanged(QString, QString)),
                     //treeDisplayWidget,
                     //SLOT(updateText(QString, QString)));

    //QObject::connect(currentTree,
                     //SIGNAL( treeChanged(QString, QString)),
                     //this,
                     //SLOT(updateGrid(QString, QString)));

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

    // Connect the various display widgets
    QObject::connect( this,
                      SIGNAL(sendUpdatedGridText(TreeState*)),
                      gridDisplayer,
                      SLOT( updateFromTreeState(TreeState*)) );

    QObject::connect( this,
                      SIGNAL( sendUpdatedTreeText(TreeState*)),
                      treeDisplayWidget,
                      SLOT( updateFromTreeState(TreeState*)) );

    // Force a redraw
    currentTree->redraw();

    /* Proof steps */
    stepOverview = new StepOverview( this, currentTree );
    ui->stepPlaceholderLayout->addWidget( stepOverview );

    QObject::connect(stepOverview,
                     SIGNAL( selectionChanged(TreeState*) ),
                     this,
                     SLOT( setNewState(TreeState*)) );
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
        qDebug() << "A-F: add statement";
        command = new CTreeStateAddStatement(currentTree,
                                             event->text().at(0).toUpper());
        commandInvoker.runCommand(command);
        return;
    }

    // Other keys will have more specific functions
    switch (event->key())
    {
    case Qt::Key_J:
        qDebug() << "J: highlight first child";
        command = new CTreeStateHighlightChild(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_K:
        qDebug() << "K: highlight parent";
        command = new CTreeStateHighlightParent(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_H:
        qDebug() << "H: highlight left";
        command = new CTreeStateHighlightLeft(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_I:
    {
        qDebug() << "I: insert from formula";
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
        qDebug() << "L: highlight right";
        command = new CTreeStateHighlightRight(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_M:
        qDebug() << "M: delete a node, but save its children";
        command = new CTreeStateDetachNodeButSaveChildren(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_N:
        qDebug() << "N: a(n)nhilate a node / delete it and all its children";
        command = new CTreeStateDetachNode(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_O:
        qDebug() << "O: surround with cut (capital letter works on individuals)";
        if (QString::compare(event->text(), "o") == 0)
            command = new CTreeStateSurroundWithCutAsGroup(currentTree);
        else
            command = new CTreeStateSurroundWithCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_P: // TODO: change this to a group maybe
        qDebug() << "P: surround with double cut";
        command = new CTreeStateSurroundWithDoubleCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_Q:
        qDebug() << "Q: enter Q mode for 1 second";
        keybindMode = constants::MODE_Q;
        QTimer::singleShot(1000,this,SLOT(endTimer()));
        break;
    case Qt::Key_R:
        qDebug() << "R: add conditional template";
        command = new CTreeStateAddConditionalTemplate(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_S:
        qDebug() << "S: enter select mode";
        keybindMode = constants::MODE_SELECT; // Enter select mode
        break;
    case Qt::Key_T:
        qDebug() << "T: add biconditional template";
        command = new CTreeStateAddBiconditionalTemplate(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_U:
        qDebug() << "U: undo the last command";
        commandInvoker.undoLastCommand();
        break;
    case Qt::Key_V:
        qDebug() << "V: adds or template";
        command = new CTreeStateAddOrTemplate(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_X:
        qDebug() << "X: adds a cut";
        command = new CTreeStateAddCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_Y:
        qDebug() << "Y: redo the last command";
        commandInvoker.redoLastCommand();
        break;
    case Qt::Key_Z:
        qDebug() << "Z: adds a double cut";
        command = new CTreeStateAddDoubleCut(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_1: // DEBUG: copy the current tree
    {
        qDebug() << "DEBUG: 1 saves a tree state to test swapping with 2 key";
        if (copiedTree != NULL)
            delete copiedTree;

        copiedTree = new TreeState(currentTree);
        break;
    }
    case Qt::Key_2: // DEBUG: swap between copied tree and current tree
    {
        qDebug() << "DEBUG: 2 swaps between tree states saved with 1 key";

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
        qDebug() << "DEBUG: 4 adds placeholder to highlighted";
        currentTree->addPlaceholderToHighlighted();
        currentTree->redraw();
        break;
    case Qt::Key_5:
        qDebug() << "DEBUG: 5 shows output string";
        qDebug() << currentTree->toOutputString();
        QApplication::clipboard()->setText(currentTree->toOutputString());
        break;
    case Qt::Key_6:
        qDebug() << "DEBUG: 6 shows standardized string";
        qDebug() << currentTree->getRoot()->getStringRep();
        break;
    //case Qt::Key_9:
    //{
        //qDebug() << "DEBUG: 9 prints grid in plaintext";
        //Grid g(currentTree);

        //ui->tempGridText->setText(g.toPlaintext());
        //break;
    //}
    case Qt::Key_Period:
        qDebug() << "( . ): repeat last command";
        commandInvoker.repeatLastCommand();
        break;
    case Qt::Key_Semicolon:
        qDebug() << "( ; ): highlight root";
        command = new CTreeStateHighlightRoot(currentTree);
        commandInvoker.runCommand(command);
        break;
    case Qt::Key_Slash:
        qDebug() << "( Slash ): mod mode";
        keybindMode = constants::MODE_MOD;
        break;
    case Qt::Key_BracketLeft:
        qDebug() << "( [ ): enter pounce mode";
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
 * Modification mode commands
 */
void MainWindow::handleKeyPressMod(QKeyEvent *event)
{
    ICommand* command;

    switch (event->key())
    {
    case Qt::Key_Escape:
        keybindMode = constants::MODE_DEFAULT;
        qDebug() << "Switching back to default mode";
        break;
    case Qt::Key_W:
        qDebug() << "(W)rap in double cut";
        command = new CModSurroundWithDoubleCut(currentTree, stepOverview );
        commandInvoker.runCommand( command );
        break;
    default:
        qDebug() << "Not a valid keypress in MOD mode";
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
    case constants::MODE_MOD:
        handleKeyPressMod(event);
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
        {
            newWindow->show();

            // If no nodes on root, close this window so that the new one is
            // the only window.
            //
            // Obviously, the actual check we'll end up needing is a bit more
            // involved (some kind of actual bool flag that ties into the Save/
            // Save As * indicator to show if something has changed), but this
            // basic check will do for now.
            //
            // I figure we'll need to do something with the commandInvoker to
            // see if commands have been run. I wrote the basic helper
            // commandInvoker->anyCommands() that returns a bool indicating if
            // at least one command was run, but this isn't exactly what I
            // needed at the moment. It needs more thought/code to make it work,
            // and if we're going to put effort into it, we might as well just
            // spend time making the actual changes verification.
            //
            // Until then, this basic check for seeing if the root has any kids
            // is good enough for me.
            //
            // TODO: better verification for detecting notable changes

            if (currentTree->getRoot()->getChildren().empty() )
                this->close();
        }
    }

}

/*
 * Save the current tree to a text file.
 *
 * TODO: make save work with full history, not just a single tree as it is now.
 */
void MainWindow::on_actionSave_triggered()
{
    FileConverter::save(this);
}

void MainWindow::setCurrState(TreeState *state)
{
    // Remove the old connection
    currentTree->disconnect();

    // Connect the new state passed in
    currentTree = state;
    connectTree();

    //QObject::connect(currentTree,
                     //SIGNAL(treeChanged(QString)),
                     //treeDisplayWidget,
                     //SLOT(updateText(QString)));

    // Draw it
    //currentTree->redraw(); // redraw calls the connected tree
    //Grid g(currentTree);
    //ui->tempGridText->setText(g.toPlaintext());

}

//void MainWindow::updateGrid(QString, QString gridText)
//{
    //ui->tempGridText->setText(gridText);

//}


void MainWindow::setNewState(TreeState *s)
{
    qDebug() << "Supposed to set a new state";
    qDebug() << "State output is " << s->toOutputString();
    // Disconnect the old one
    currentTree->disconnect();

    // Connect up the new one
    currentTree = s;
    connectTree();

    // Force redraw
    currentTree->redraw();
}

void MainWindow::currTreeNeedsRedraw()
{
    qDebug() << "I should redraw the current tree!";

    // Emit the signal to the GridDisplayer
    qDebug() << "Emit grid displayer";
    emit( sendUpdatedGridText( currentTree ) );

    // Emit the signal to the TreeDisplayWidget
    qDebug() << "Emit tree displayer";
    emit( sendUpdatedTreeText( currentTree ) );
}

void MainWindow::connectTree()
{
    // Connect the currentTree
    QObject::connect( currentTree,
                      SIGNAL( treeChanged() ),
                      this,
                      SLOT( currTreeNeedsRedraw() ) );

    //QObject::connect(currentTree,
                     //SIGNAL(treeChanged(TreeState*)),
                     //this,
                     //SLOT(setNewState(TreeState*)) );
}
