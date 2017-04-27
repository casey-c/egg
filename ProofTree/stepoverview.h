#ifndef STEPOVERVIEW_H
#define STEPOVERVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include "clickableframe.h"
#include "goalitem.h"
#include "stepitem.h"

#include "Tree/treestate.h"

namespace Ui {
class StepOverview;
}

class StepOverview : public QWidget
{
    Q_OBJECT

public:
    explicit StepOverview(QWidget *parent, TreeState* premise);
    ~StepOverview();

    void selectChild( ClickableFrame* child );

    void selectStepItem( StepItem* step );
    void selectGoalItem( GoalItem* goal );

    void addStep( QString text, TreeState* state );

private slots:
    void on_pushButton_clicked();

signals:
    void selectionChanged( TreeState* );

private:
    Ui::StepOverview *ui;

    ClickableFrame* selected;

    int lastStepCounter;

    void scrollToBottom();

    GoalItem* goalItem;
    QVBoxLayout* scrollLayout;
};

#endif // STEPOVERVIEW_H
