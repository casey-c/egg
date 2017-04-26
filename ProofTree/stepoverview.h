#ifndef STEPOVERVIEW_H
#define STEPOVERVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include "clickableframe.h"
#include "goalitem.h"

namespace Ui {
class StepOverview;
}

class StepOverview : public QWidget
{
    Q_OBJECT

public:
    explicit StepOverview(QWidget *parent = 0);
    ~StepOverview();

    void selectChild( ClickableFrame* child );

private slots:
    void on_pushButton_clicked();

private:
    Ui::StepOverview *ui;

    ClickableFrame* selected;

    int lastStepCounter;

    GoalItem* goalItem;
    QVBoxLayout* scrollLayout;
};

#endif // STEPOVERVIEW_H
