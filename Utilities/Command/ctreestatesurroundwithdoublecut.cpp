#include "ctreestatesurroundwithdoublecut.h"

/* Surrounds everything in the selection with double cuts */
bool CTreeStateSurroundWithDoubleCut::execute()
{
    // Remember the old highlighted
    prevHighlighted = tree->getHighlighted();

    // Surround the selection with cuts
    tree->surroundWithCut();
    addedInnerCuts = tree->popRecentNodes();

    // Check if anything was added
    if (addedInnerCuts.isEmpty())
        return false;

    // Surround the new cuts with cuts of their own
    for (TreeNode* node : addedInnerCuts)
        tree->selectSpecific(node);

    tree->surroundWithCut();
    addedOuterCuts = tree->popRecentNodes();

    tree->redraw();
    return true;
}

/* Removes any added double cuts */
void CTreeStateSurroundWithDoubleCut::undo()
{
    tree->highlightSpecific(prevHighlighted);

    for (TreeNode* node : addedInnerCuts)
        tree->removeAndSaveOrphans(node);

    for (TreeNode* node : addedOuterCuts)
        tree->removeAndSaveOrphans(node);

    tree->redraw();
}
