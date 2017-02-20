#ifndef MAINWINDOW_MAINWINDOW_H
#define MAINWINDOW_MAINWINDOW_H

#include <QMainWindow>
#include "Tree/treestate.h"

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
};

#endif // MAINWINDOW_MAINWINDOW_H
