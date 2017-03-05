#ifndef CTREESTATESURROUNDWITHDOUBLECUT_H
#define CTREESTATESURROUNDWITHDOUBLECUT_H

#include <QList>

#include "Tree/treestate.h"
#include "Tree/treenode.h"

class CTreeStateSurroundWithDoubleCut : public ICommand
{
public:
    /* Constructor */
    CTreeStateSurroundWithDoubleCut(TreeState* t):
        tree(t) { text = "Surround with double cut"; }

    /* Destructor */
    ~CTreeStateSurroundWithDoubleCut();

    bool execute();
    void undo();

    ICommand* copy() { return new CTreeStateSurroundWithDoubleCut(tree); }

private:
    TreeState* tree;

    TreeNode* prevHighlighted;
    QList<TreeNode*> addedOuterCuts;
    QList<TreeNode*> addedInnerCuts;
};

#endif // CTREESTATESURROUNDWITHDOUBLECUT_H
