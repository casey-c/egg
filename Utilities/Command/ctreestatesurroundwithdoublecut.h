#ifndef CTREESTATESURROUNDWITHDOUBLECUT_H
#define CTREESTATESURROUNDWITHDOUBLECUT_H

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
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;

    TreeNode* outerCut;
    TreeNode* innerCut;
};

#endif // CTREESTATESURROUNDWITHDOUBLECUT_H
