#include "itreestateremove.h"

/* Perform a removal operation on a tree state */
bool ITreeStateRemove::execute()
{
    // Remember the old selections and highlight
    prevHighlighted = tree->getHighlighted();
    prevSelected = tree->getSelectionList();

    // Perform the remove call
    remove();

    removedNodes = tree->popRecentNodes();
    oldParents = tree->popRecentParents();

    // Verify that the removed nodes match up evenly with the old parents. If
    // something goes wrong, we know we have an issue with the remove() call
    if (removedNodes.size() != oldParents.size())
    {
        qDebug() << "ERROR: nodes not correctly matched with parents";
        return false;
    }

    // Check if anything changed
    if (removedNodes.size() == 0 || oldParents.size() == 0)
        return false;

    tree->redraw();
    return true;
}

/* Puts the detached nodes back into place */
void ITreeStateRemove::undo()
{
    // Repeat the same check that's in execute(), for paranoia reasons
    if (removedNodes.size() != oldParents.size())
        return;

    qDebug() << "attempting to move back into place";

    // Move the nodes back into place
    QList<TreeNode*>::iterator nodeIt = removedNodes.begin();
    QList<TreeNode*>::iterator parentIt = oldParents.begin();

    for (; nodeIt != removedNodes.end(); ++nodeIt, ++parentIt)
        TreeNode::move((*nodeIt),(*parentIt));

    // Reselect and rehighlight
    qDebug() << "rehighlighting";
    tree->highlightSpecific(prevHighlighted);
    for (TreeNode* node : prevSelected)
         tree->selectSpecific(node);

    tree->redraw();
}
