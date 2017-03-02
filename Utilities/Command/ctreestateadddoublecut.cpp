#include "ctreestateadddoublecut.h"

/* Adds double cut */
bool CTreeStateAddDoubleCut::execute()
{
    TreeNode* previous = tree->getSelected();
    node = tree->addChildCut();

    // No change
    if (node == previous)
        return false;

    tree->addChildCut();
    tree->redraw();
    return true;
}

/* Removes the added double cut */
void CTreeStateAddDoubleCut::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the double cut */
void CTreeStateAddDoubleCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddDoubleCut::copy()
{
    return new CTreeStateAddDoubleCut(tree);
}
