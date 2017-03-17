#include "polishinputwidget.h"
#include "ui_polishinputwidget.h"

PolishInputWidget::PolishInputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolishInputWidget)
{
    ui->setupUi(this);
    QWidget::setWindowFlags(Qt::Dialog);
}

PolishInputWidget::~PolishInputWidget()
{
    delete ui;
}
