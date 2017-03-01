#include "ctreestatesurroundwithcut.h"

/* Surrounds with cut */
void CTreeStateSurroundWithCut::execute()
{
    node = tree->getSelected();
    cut = tree->surroundWithCut();
    tree->redraw();
}

/* Moves the inside node back to the parent and deletes the cut */
void CTreeStateSurroundWithCut::undo()
{
    // TODO:
    // use the other removeAndSaveTheorphans func,
    // so we don't need to perform any moves!

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

/* Resurrounds the node with the cut */
void CTreeStateSurroundWithCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSurroundWithCut::copy()
{
    return new CTreeStateSurroundWithCut(tree);
}
