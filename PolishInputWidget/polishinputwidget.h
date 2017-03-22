#ifndef POLISHINPUTWIDGET_H
#define POLISHINPUTWIDGET_H

#include <QWidget>
#include <QStack>

#include "Tree/treestate.h"
#include "PolishTree/polishnode.h"

namespace Ui {
class PolishInputWidget;
}

/*
 * This widget serves as the pop-up window for inserting a formula (I key). It
 * uses polish notation to build a PolishTree, while simultaneously converting
 * input instructions into a existential graph tree as well.
 *
 * Once a WFF is produced, the ADD button becomes clickable, and clicking it
 * will move all the nodes from the constructed tree into the real one as
 * children of the currently highlighted node.
 *
 * This widget also essentially exists as a polish-tree equivalent to the
 * TreeState. It stores the root PolishNodes and maintains the input stacks
 * needed for displaying the polish notation outputs.
 */
class PolishInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PolishInputWidget(QWidget *parent = 0);
    ~PolishInputWidget();

private:
    /* UI */
    Ui::PolishInputWidget *ui;

    /* Roots */
    PolishNode* polishRoot;
    TreeNode* eggRoot;

    /* Stacks */
    QStack<PolishNode*> polishStack;
    QStack< QList<TreeNode*> > eggStack;

    /* Changes */
    void addNArityNode(int arity, QString text);
    void redraw();

    /* Keybinds */
    void keyPressEvent(QKeyEvent* event);

    /* Details */
    bool wff;
    bool closeEarly;

private slots:
    /* Button press on completion */
    void on_add_button_clicked();

signals:
    /* Exit this widget and send details to the MainWindow */
    void sendCompletedFormula(TreeNode* root);
};

#endif // POLISHINPUTWIDGET_H
