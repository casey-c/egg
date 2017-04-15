#include "prooftreenodeview.h"
#include "ui_prooftreenodeview.h"

ProofTreeNodeView::ProofTreeNodeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProofTreeNodeView)
{
    ui->setupUi(this);
}

ProofTreeNodeView::~ProofTreeNodeView()
{
    delete ui;
}
