#include "MainWindow/mainwindow.h"
#include "ui_mainwindow.h"

#include "Tree/treestate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    currentTree = new TreeState();
}

MainWindow::~MainWindow()
{
    delete ui;
}
