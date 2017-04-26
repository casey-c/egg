#ifndef GOALITEM_H
#define GOALITEM_H

#include "clickableframe.h"

#include <QWidget>
#include <QLabel>

namespace Ui
{
class GoalItem;
}

class GoalItem : public ClickableFrame
{
    Q_OBJECT

public:
    explicit GoalItem( QWidget* parent = 0 );

    void updateIcon();

private:
    QLabel* iconLabel;
    QPixmap xIcon;
    QPixmap checkIcon;

    void onSuccessfulClick();

    bool isGood;
};

#endif // GOALITEM_H
