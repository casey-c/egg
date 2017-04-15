#include "treedisplaywidget.h"
#include "ui_treedisplaywidget.h"
#include <QDebug>

TreeDisplayWidget::TreeDisplayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TreeDisplayWidget)
{
    ui->setupUi(this);
}

TreeDisplayWidget::~TreeDisplayWidget()
{
    delete ui;
}

void TreeDisplayWidget::updateText(QString s, QString)
{
    ui->textEdit->setText(s);
}
