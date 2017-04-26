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

StepOverview::StepOverview(QWidget *parent, TreeState* premise) :
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

    //scrollLayout->addWidget(new StepItem( this,
                                          //lastStepCounter++,
                                          //"Premise",
                                          //premise ) );

    addStep("Premise", premise);
}


StepOverview::~StepOverview()
{
    delete ui;
}

void StepOverview::selectChild(ClickableFrame *child)
{
    qDebug() << "step overview select child";
    // No change in selection
    if ( selected == child )
        return;
    qDebug() << "selection changed";

    // Remove old selection
    if ( selected != nullptr )
        selected->deselect();

    qDebug() << "no current selection ";

    selected = child;
    selected->select();

    qDebug() << "selected the new current";

    // Emit the signal to display
    //if ( StepItem* c = dynamic_cast<StepItem*>(selected))
    //{
        //qDebug() << "Cast to stepItem succeeded";
        //emit( selectionChanged( c->state() ) );
    //}
    //else
    //{
        //qDebug() << "Cast to stepItem failed";
    //}
}

void StepOverview::selectStepItem(StepItem *step)
{
    selectChild(step);
    qDebug() << "Emit step here";
    emit( selectionChanged(step->state()) );
}

void StepOverview::selectGoalItem(GoalItem *goal)
{
    selectChild(goal);
    qDebug() << "Emit goal here";
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
    qDebug() << "Stepoverview: addstep";
    StepItem* newItem = new StepItem( this,
                                      lastStepCounter++,
                                      text,
                                      state );
    scrollLayout->addWidget( newItem );
    selectStepItem(newItem);
    //newItem->select();
    //selectChild( newItem );
}
