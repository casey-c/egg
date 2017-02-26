#ifndef MAINWINDOW_MAINWINDOW_H
#define MAINWINDOW_MAINWINDOW_H

#include <QMainWindow>
#include "Tree/treestate.h"
#include "TreeDisplayWidget/treedisplaywidget.h"
#include "Utilities/Command/commandinvoker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TreeState* currentTree;
    TreeDisplayWidget* treeDisplayWidget;

    /* Commands */
    CommandInvoker commandInvoker;

    /* Timer */
    int keybindMode;

    /* Key press handlers */
    void keyPressEvent(QKeyEvent *event);
    void handleKeyPressDefault(QKeyEvent *event);
    void handleKeyPressQ(QKeyEvent *event);

private slots:
    void endTimer();
public slots:
    void updateUndoMenu(QString undo, QString redo, QString repeat,
                        bool undoable, bool redoable, bool repeatable);

};

#endif // MAINWINDOW_MAINWINDOW_H
