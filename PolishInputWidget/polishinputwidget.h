#ifndef POLISHINPUTWIDGET_H
#define POLISHINPUTWIDGET_H

#include <QWidget>
#include <QStack>

#include "Tree/treestate.h"
#include "PolishTree/polishnode.h"

namespace Ui {
class PolishInputWidget;
}

class PolishInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PolishInputWidget(QWidget *parent = 0);
    ~PolishInputWidget();

private slots:
    void on_add_button_clicked();

private:
    Ui::PolishInputWidget *ui;

    void keyPressEvent(QKeyEvent* event);

    PolishNode* polishRoot;
    TreeNode* eggRoot;

    QStack<PolishNode*> polishStack;
    //QStack<TreeNode*> eggStack;
    QStack< QList<TreeNode*> > eggStack;
    bool wff;

    void addNArityNode(int arity, QString text);
    void redraw();

    bool closeEarly;

signals:
    void sendCompletedFormula(TreeNode* root);
};

#endif // POLISHINPUTWIDGET_H
