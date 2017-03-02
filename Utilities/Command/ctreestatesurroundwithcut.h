#ifndef CTREESTATESURROUNDWITHCUT_H
#define CTREESTATESURROUNDWITHCUT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command surrounds a selection in a TreeState with a cut
 */
class CTreeStateSurroundWithCut : public ICommand
{
public:
    /* Constructor */
    CTreeStateSurroundWithCut(TreeState* t):
        tree(t) { text = "Surround with cut"; }

    /* Destructor */
    ~CTreeStateSurroundWithCut();

    bool execute();
    void undo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;
    TreeNode* cut;
};

#endif // CTREESTATESURROUNDWITHCUT_H
