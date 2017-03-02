#include "ctreestatesurroundwithcut.h"

/* Surrounds with cut */
bool CTreeStateSurroundWithCut::execute()
{
    node = tree->getSelected();
    cut = tree->surroundWithCut();

    // Couldn't surround with cut
    if (cut == NULL)
        return false;

    tree->redraw();
    return true;
}

/* Moves the inside node back to the parent and deletes the cut */
void CTreeStateSurroundWithCut::undo()
{
    // No cut was performed, so nothing to undo!
    if (cut == NULL)
        return;

    // Perform the move
    TreeNode* parent = cut->getParent();
    tree->move(node,parent);

    // Delete the cut
    tree->selectSpecific(cut);
    tree->removeAndBurnTheOrphanage();

    // Reselect the proper node and redraw
    tree->selectSpecific(node);
    tree->redraw();
}

/* Copy */
ICommand* CTreeStateSurroundWithCut::copy()
{
    return new CTreeStateSurroundWithCut(tree);
}
