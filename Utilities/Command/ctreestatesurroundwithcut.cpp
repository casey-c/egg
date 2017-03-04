#include "ctreestatesurroundwithcut.h"

/* Surrounds with cut */
bool CTreeStateSurroundWithCut::execute()
{
    // Surround all the selected nodes with double cuts, if possible
    tree->surroundWithCut();

    // Determine which nodes we surrounded
    addedCuts = tree->popRecentNodes();

    // Check if we did anything
    if (addedCuts.isEmpty())
        return false;

    tree->redraw();
    return true;
}

/* Moves the inside node back to the parent and deletes the cut */
void CTreeStateSurroundWithCut::undo()
{
    // Revert the changes by deleting the cuts and moving the nodes to the
    // cuts' parents
    for (TreeNode* node : addedCuts)
        tree->removeAndSaveOrphans(node);

    tree->redraw();
}
