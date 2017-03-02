#ifndef CTREESTATEADDDOUBLECUT_H
#define CTREESTATEADDDOUBLECUT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command adds a double cut to a TreeState
 */
class CTreeStateAddDoubleCut : public ICommand
{
public:
    /* Constructor */
    CTreeStateAddDoubleCut(TreeState* t) :
        tree(t) { text = "Add double cut"; }

    /* Destructor */
    ~CTreeStateAddDoubleCut() {}

    bool execute();
    void undo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDDOUBLECUT_H
