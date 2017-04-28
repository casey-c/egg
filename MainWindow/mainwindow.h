#ifndef MAINWINDOW_MAINWINDOW_H
#define MAINWINDOW_MAINWINDOW_H

#include <QMainWindow>
#include "Tree/treestate.h"
#include "TreeDisplayWidget/treedisplaywidget.h"
#include "Utilities/Command/commandinvoker.h"
#include "ProofTree/stepoverview.h"

#include "Grid/griddisplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* Load */
    void setCurrState(TreeState* state);

    /* Save */
    TreeState* getCurrentTree() { return currentTree; }

private:
    Ui::MainWindow *ui;
    TreeState* currentTree;
    TreeState* copiedTree; // For debugging copy constructor, to be removed
    TreeDisplayWidget* treeDisplayWidget;

    /* Commands */
    CommandInvoker commandInvoker;

    /* Timer */
    int keybindMode;

    /* Key press handlers */
    void keyPressEvent(QKeyEvent* event);
    void handleKeyPressDefault(QKeyEvent* event);
    void handleKeyPressQ(QKeyEvent* event);
    void handleKeyPressSelect(QKeyEvent* event);
    void handleKeyPressPounce(QKeyEvent* event);
    void handleKeyPressMod(QKeyEvent* event);

    /* Pounce */
    bool typedOne;
    QString pounceTarget;

    /* Grid display */
    GridDisplayer* gridDisplayer;

    /* Mod */
    StepOverview* stepOverview;

    void connectTree();

private slots:
    void endTimer();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    /* Change or redraw the current tree state */
    void currTreeNeedsRedraw();
    void setNewState( TreeState* s );

    void on_actionNew_triggered();

signals:
    /* These connect to the GridDisplayer and TreeView widgets */
    void sendUpdatedGridText( TreeState* t );
    void sendUpdatedTreeText( TreeState* t );

public slots:
    void updateUndoMenu(QString undo, QString redo, QString repeat,
                        bool undoable, bool redoable, bool repeatable);

    void insertFromFormula(TreeNode* root);

    //void updateGrid(QString, QString gridText);

    //void updateState( QString tree, QString grid );

};

#endif // MAINWINDOW_MAINWINDOW_H
