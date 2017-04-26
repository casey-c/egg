#ifndef TREEDISPLAYWIDGET_H
#define TREEDISPLAYWIDGET_H

#include <QWidget>
#include <QString>

#include "Tree/treestate.h"

namespace Ui {
class TreeDisplayWidget;
}

class TreeDisplayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TreeDisplayWidget(QWidget *parent = 0);
    ~TreeDisplayWidget();

public slots:
    void updateText(QString s, QString);

    void updateFromTreeState( TreeState* t );

private:
    Ui::TreeDisplayWidget *ui;
};

#endif // TREEDISPLAYWIDGET_H
