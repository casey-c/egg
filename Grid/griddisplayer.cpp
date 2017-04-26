#include "griddisplayer.h"
#include "ui_griddisplayer.h"
#include "Grid/grid.h"

GridDisplayer::GridDisplayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GridDisplayer)
{
    ui->setupUi(this);
}

GridDisplayer::~GridDisplayer()
{
    delete ui;
}

void GridDisplayer::updateFromTreeState(TreeState *tree)
{
    qDebug() << "GridDisplayer: update from tree state";

    Grid g(tree);
    ui->textEdit->setText( g.toPlaintext() );
}
