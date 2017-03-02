#ifndef CTREESTATEADDCUT_H
#define CTREESTATEADDCUT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command adds a cut to a TreeState
 */
class CTreeStateAddCut : public ICommand
{
public:
    /* Constructor */
    CTreeStateAddCut(TreeState* t) :
        tree(t) { text = "Add cut"; }

    /* Destructor */
    ~CTreeStateAddCut() {}

    bool execute();
    void undo();
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDCUT_H
