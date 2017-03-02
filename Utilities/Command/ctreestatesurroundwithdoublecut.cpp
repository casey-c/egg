#include "ctreestatesurroundwithdoublecut.h"

/* Surrounds with double cut */
bool CTreeStateSurroundWithDoubleCut::execute()
{
    node = tree->getSelected();
    outerCut = tree->surroundWithCut();

    // Couldn't surround with cut
    if (outerCut == NULL)
        return false;

    innerCut = tree->surroundWithCut();
    tree->redraw();
    return true;
}

/* Moves the inside node back to its grandparent and deletes both cuts */
void CTreeStateSurroundWithDoubleCut::undo()
{
    // Didn't make any cuts
    if (outerCut == NULL || innerCut == NULL)
        return;

    // Perform the move
    TreeNode* grandparent = outerCut->getParent();
    tree->move(node,grandparent);

    // Delete the cuts
    tree->selectSpecific(outerCut);
    tree->removeAndBurnTheOrphanage();

    // Reselect the proper node and redraw
    tree->selectSpecific(node);
    tree->redraw();
}

/* Readds the double cut */
void CTreeStateSurroundWithDoubleCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSurroundWithDoubleCut::copy()
{
    return new CTreeStateSurroundWithDoubleCut(tree);
}
