#include "Utilities/Command/ctreestatedetachnodebutsavechildren.h"

/*
 * Detaches a certain node but moves all its children to their grandparents
 */
bool CTreeStateDetachNodeButSaveChildren::execute()
{
    // Remember the old selections and highlight
    prevHighlighted = tree->getHighlighted();
    prevSelected = tree->getSelectionList();

    // Do the main work
    tree->detachNodeAndMoveOrphans();

    detachedNodes = tree->popRecentNodes();
    oldParents = tree->popRecentParents();
    relocatedChildren = tree->popRecentChildren();

    // Verify that all three (parallel) lists are the same size
    if (detachedNodes.size() != oldParents.size()
            || detachedNodes.size() != relocatedChildren.size())
    {
        qDebug() << "ERROR: lists not correctly parallel";
        return false;
    }

    // Check if anything changed
    if (detachedNodes.size() == 0)
        return false;

    tree->redraw();
    return true;
}

/*
 * Reconnects the detached node, and gives back its moved children
 */
void CTreeStateDetachNodeButSaveChildren::undo()
{
    // Move the detached nodes back into place
    QList<TreeNode*>::iterator nodeIt = detachedNodes.begin();
    QList<TreeNode*>::iterator parentIt = oldParents.begin();

    for (; nodeIt != detachedNodes.end(); ++nodeIt, ++parentIt)
        TreeNode::move((*nodeIt),(*parentIt));

    // Move the children back into place
    nodeIt = detachedNodes.begin();
    QList< QList<TreeNode*> >::iterator childIt = relocatedChildren.begin();

    for (;nodeIt != detachedNodes.end(); ++nodeIt, ++childIt)
        for (TreeNode* child : (*childIt))
            TreeNode::move(child, (*nodeIt));

    // Reselect and rehighlight
    tree->highlightSpecific(prevHighlighted);
    for (TreeNode* node : prevSelected)
        tree->selectSpecific(node);

    tree->redraw();
}
