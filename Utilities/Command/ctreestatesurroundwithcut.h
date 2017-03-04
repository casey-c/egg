#ifndef CTREESTATESURROUNDWITHCUT_H
#define CTREESTATESURROUNDWITHCUT_H

#include <QList>

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command surrounds all selections in a TreeState with a cut
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

    ICommand* copy() { return new CTreeStateSurroundWithCut(tree); }

private:
    TreeState* tree;
    QList<TreeNode*> addedCuts;
};

#endif // CTREESTATESURROUNDWITHCUT_H
