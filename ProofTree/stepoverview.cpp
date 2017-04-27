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
    // No change in selection
    if ( selected == child )
        return;

    // Remove old selection
    if ( selected != nullptr )
        selected->deselect();

    selected = child;
    selected->select();
}

void StepOverview::selectStepItem(StepItem *step)
{
    selectChild(step);
    emit( selectionChanged(step->state()) );
}

void StepOverview::selectGoalItem(GoalItem *goal)
{
    selectChild(goal);
    qDebug() << "TODO: Emit goal here";
}

void StepOverview::on_pushButton_clicked()
{
    scrollLayout->addWidget(new StepItem(this,
                                         lastStepCounter++,
                                         getRandStepType(),
                                         new TreeState()));

    scrollToBottom();
}

void StepOverview::addStep(QString text, TreeState *state)
{
    StepItem* newItem = new StepItem( this,
                                      lastStepCounter++,
                                      text,
                                      state );
    scrollLayout->addWidget( newItem );
    selectStepItem(newItem);
    scrollToBottom();
}

void StepOverview::scrollToBottom()
{
    qDebug() << "Attempting to scroll to bottom";

    // Scroll to bottom (kinda ugly)
    ui->scrollArea->widget()->adjustSize();
    qApp->processEvents();
    ui->scrollArea->verticalScrollBar()->setValue(
                ui->scrollArea->verticalScrollBar()->maximum() );
}
