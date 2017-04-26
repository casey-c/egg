#ifndef STEPITEM_H
#define STEPITEM_H

#include <QWidget>
#include <QLabel>
#include "clickableframe.h"
#include "Tree/treestate.h"

namespace Ui {
class StepItem;
}

class StepItem : public ClickableFrame
{
    Q_OBJECT

public:
    explicit StepItem(QWidget* parent, int c, QString s, TreeState* state );

private:
    QLabel* numLabel;
    QLabel* text;

    TreeState* correspondingTree;

    /* Overridden from ClickableFrame */
    void onSuccessfulClick();
};

#endif // STEPITEM_H
