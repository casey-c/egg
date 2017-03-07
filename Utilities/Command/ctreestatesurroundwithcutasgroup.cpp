#include "ctreestatesurroundwithcutasgroup.h"

/* Surrounds a group with cut */
bool CTreeStateSurroundWithCutAsGroup::execute()
{
    // Surround all the selected nodes with a single cut
    tree->surroundWithCutAsGroup();

    // Determine which nodes we surrounded
    addedCuts = tree->popRecentNodes();

    // Check if we did anything
    if (addedCuts.isEmpty())
        return false;

    tree->redraw();
    return true;
}

/* Deletes the newest cut */
void CTreeStateSurroundWithCutAsGroup::undo()
{
    // Reverts the changes
    for (TreeNode* node : addedCuts)
        tree->removeAndSaveOrphans(node);

    tree->redraw();
}
