#ifndef CTREESTATESURROUNDWITHCUTASGROUP_H
#define CTREESTATESURROUNDWITHCUTASGROUP_H

#include <QList>

#include "Tree/treestate.h"
#include "Tree/treenode.h"

class CTreeStateSurroundWithCutAsGroup : public ICommand
{
public:
    /* Constructor */
    CTreeStateSurroundWithCutAsGroup(TreeState* t) :
        tree(t) { text = "Surround group with cut"; }

    /* Destructor */
    ~CTreeStateSurroundWithCutAsGroup();

    bool execute();
    void undo();

    ICommand* copy() { return new CTreeStateSurroundWithCutAsGroup(tree); }

private:
    TreeState* tree;
    QList<TreeNode*> addedCuts;
};

#endif // CTREESTATESURROUNDWITHCUTASGROUP_H
