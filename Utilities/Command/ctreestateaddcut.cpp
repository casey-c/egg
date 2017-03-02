#include "ctreestateaddcut.h"

/* Adds child cut */
bool CTreeStateAddCut::execute()
{
    TreeNode* previous = tree->getSelected();
    node = tree->addChildCut();

    // No change
    if (previous == node)
        return false;

    tree->redraw();
    return true;
}

/* Removes the added cut */
void CTreeStateAddCut::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the cut */
void CTreeStateAddCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddCut::copy()
{
    return new CTreeStateAddCut(tree);
}

