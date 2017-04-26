#include "stepoverview.h"
#include "ui_stepoverview.h"
#include "stepitem.h"
#include "goalitem.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QScrollBar>

QString getRandStepType()
{
    int r = qrand() % 6;
    switch (r)
    {
    case 0:
        return "Double-cut addition";
    case 1:
        return "Double-cut removal";
    case 2:
        return "Iteration";
    case 3:
        return "Deiteration";
    case 4:
        return "Insertion";
    case 5:
        return "Erasure";
    default:
        return "Double-cut surround";
    }
}

StepOverview::StepOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepOverview),
    selected( nullptr ),
    lastStepCounter( 0 )
{
    ui->setupUi(this);

    scrollLayout = new QVBoxLayout( ui->scrollArea );
    scrollLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    scrollLayout->setSpacing(7);

    QWidget* inner = new QWidget( ui->scrollArea );
    ui->scrollArea->setWidget( inner );
    inner->setLayout( scrollLayout );

    goalItem = new GoalItem(this);
    ui->goalLayout->addWidget(goalItem);

    scrollLayout->addWidget(new StepItem(this,
                                         lastStepCounter++,
                                         "Premise",
                                         new TreeState()));
}


StepOverview::~StepOverview()
{
    delete ui;
}

void StepOverview::selectChild(ClickableFrame *child)
{
    // No change in selection
    if ( selected == child )
        return;

    // Remove old selection
    if ( selected != nullptr )
        selected->deselect();

    selected = child;
    selected->select();
}

void StepOverview::on_pushButton_clicked()
{
    scrollLayout->addWidget(new StepItem(this,
                                         lastStepCounter++,
                                         getRandStepType(),
                                         new TreeState()));

    // Scroll to bottom (kinda ugly)
    ui->scrollArea->widget()->adjustSize();
    qApp->processEvents();
    ui->scrollArea->verticalScrollBar()->setValue(
                ui->scrollArea->verticalScrollBar()->maximum() );
}

void StepOverview::addStep(QString text, TreeState *state)
{
    scrollLayout->addWidget(new StepItem(this,
                                         lastStepCounter++,
                                         text,
                                         state));

}
