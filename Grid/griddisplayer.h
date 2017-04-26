#ifndef GRIDDISPLAYER_H
#define GRIDDISPLAYER_H

#include <QWidget>
#include "Tree/treestate.h"

namespace Ui {
class GridDisplayer;
}

class GridDisplayer : public QWidget
{
    Q_OBJECT

public:
    explicit GridDisplayer(QWidget *parent = 0);
    ~GridDisplayer();

private:
    Ui::GridDisplayer *ui;

public slots:
    void updateFromTreeState( TreeState* tree );
};

#endif // GRIDDISPLAYER_H
