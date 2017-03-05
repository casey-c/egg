#include "itreestateadd.h"

/*
 * Adds something to the tree state
 */
bool ITreeStateAdd::execute()
{
    // Save the previous info so we can revert there
    prevSelected = tree->getSelectionList();
    prevHighlighted = tree->getHighlighted();

    // Add what we want to add
    add();

    // Save which nodes we added
    newlyCreated = tree->popRecentNodes();

    // No change
    if (newlyCreated.isEmpty())
        return false;

    tree->redraw();
    return true;
}

/*
 * Reverts whatever was added
 */
void ITreeStateAdd::undo()
{
    // Remove the added nodes
    for (TreeNode* node : newlyCreated)
        tree->removeAndBurnTheOrphanage(node);

    // Reselect the old nodes
    tree->highlightSpecific(prevHighlighted);
    tree->clearSelection();
    for (TreeNode* node : prevSelected)
        tree->selectSpecific(node);

    tree->redraw();
}
