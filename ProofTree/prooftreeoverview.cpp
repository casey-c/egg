#include "prooftreeoverview.h"
#include "ui_prooftreeoverview.h"
#include "prooftreenodeview.h"

ProofTreeOverview::ProofTreeOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProofTreeOverview)
{
    ui->setupUi(this);

    //QWidget* inner = new QWidget(ui->scrollArea);
    //QWidget* inner = new QWidget;
    //layout = new QVBoxLayout(inner);
    //inner->setLayout(layout);

    //ui->scrollArea->setWidget(inner);

    layout = new QVBoxLayout(ui->scrollArea);

    //inner->setLayout(layout);

    //ui->scrollArea->setWidget(inner);
    ui->scrollArea->setWidget(layout->widget());
    ui->scrollArea->setWidgetResizable(false);

    ProofTreeNodeView* premise = new ProofTreeNodeView();
    layout->addWidget(premise);
}

ProofTreeOverview::~ProofTreeOverview()
{
    delete ui;
}

void ProofTreeOverview::on_pushButton_clicked()
{
    ProofTreeNodeView* newNode = new ProofTreeNodeView();
    layout->addWidget(newNode);
}
