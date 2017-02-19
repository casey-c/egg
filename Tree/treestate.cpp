#include "Tree/treestate.h"

/* TODO: implement graphstate.cpp */

/* Selects the first child of the selected region, if it exists */
void TreeState::selectAChild()
{
    if (!selected->getChildren().isEmpty())
        selected = selected->getChildren().first();
}

/* Selects the parent of the selected node, if not already root. */
void TreeState::selectParent()
{
    if (!selected->isRoot())
        selected = selected->getParent();
}

/* Recursively selects the parent of the selection until we reach root */
void TreeState::selectRoot()
{
    while (!selected->isRoot())
        selected = selected->getParent();
}

/* Selects the previous sibling of the parent's getChildren list */
void TreeState::selectLeftSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
        return;

    /* List has the selected item and all its siblings */
    QList<TreeNode*> list = selected->getParent()->getChildren();

    TreeNode* previous = NULL;

    for (TreeNode* prev : list)
    {
        if (prev == selected)
            break;
        previous = prev;
    }

    /* Nothing to the left */
    if (previous == NULL)
       selected = list.last();
    else /* Choose the previous as the new selection */
        selected = previous;
}

/* Select the next right sibling */
void TreeState::selectRightSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
        return;

    /* List has the selected item and all its siblings */
    QList<TreeNode*> list = selected->getParent()->getChildren();

    TreeNode* next = NULL;
    QList<TreeNode*>::const_reverse_iterator it = list.rbegin();
    for (; it != list.rend(); ++it)
    {
        if (*it == selected)
            break;
        next = *it;
    }

    /* Nothing to the right */
    if (next == NULL)
       selected = list.front();
    else /* Choose the next as the new selection */
        selected = next;
}

/* Adds a cut to the selected node's children */
void TreeState::addChildCut()
{
    selected = selected->addChildCut();
}

/* Adds a double cut to the selected region */
void TreeState::addChildDoubleCut()
{
    selected = selected->addChildCut();
    selected = selected->addChildCut();
}

/* Adds a child statement with the string s */
void TreeState::addChildStatement(QString s)
{
    selected = selected->addChildStatement(s);
}

/* Remove */
void TreeState::removeAndSaveOrphans()
{
    QList<TreeNode*> children = selected->getChildren();
    TreeNode* parent = selected->getParent();

    parent->addAll(children);

    // Delete the selected node
    selected->remove();

}

/* Remove the selected node and any of its children */
void TreeState::removeAndBurnTheOrphanage()
{
    selected->remove();
}

/* Surround with cut */
void TreeState::surroundWithCut()
{

}

/* Surround with double cut */
void TreeState::surroundWithDoubleCut()
{

}
