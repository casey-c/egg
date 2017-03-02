#ifndef CTREESTATESELECTROOT_H
#define CTREESTATESELECTROOT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command will call the select root function on a tree state.
 */
class CTreeStateSelectRoot : public ICommand
{
public:
    /* Constructor */
    CTreeStateSelectRoot(TreeState* t) :
        tree(t) { text = "Select root"; }

    /* Destructor */
    ~CTreeStateSelectRoot() {}

    bool execute();
    void undo();
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* previous;
};

#endif // CTREESTATESELECTROOT_H
