#include "Tree/treestate.h"
#include <QDebug>

TreeState::~TreeState()
{
    selected = NULL;
    delete root;
}

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

    emit treeChanged(getBoxedString());
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

    emit treeChanged(getBoxedString());
}

/* Select a node (used by commands) */
void TreeState::selectSpecific(TreeNode* node)
{
    selected = node;
}

/* Adds a cut to the selected node's children */
TreeNode* TreeState::addChildCut()
{
    selected = selected->addChildCut();
    return selected;
}

/* Adds a double cut to the selected region */
TreeNode* TreeState::addChildDoubleCut()
{
    addChildCut();
    return addChildCut();
}

/* Adds a child statement with the string s */
TreeNode* TreeState::addChildStatement(QString s)
{
    selected = selected->addChildStatement(s);
    return selected;
}

/* Adds an or template */
TreeNode* TreeState::addOrTemplate()
{
    selected = selected->addChildCut();
    selected->addChildCut();
    selected = selected->addChildCut();
    return selected;
}

/* Adds a conditional template */
TreeNode* TreeState::addConditionalTemplate()
{
    selected = selected->addChildCut();
    selected->addChildCut();
    selected = selected->addChildPlaceholder();
    return selected;
}

/* Remove */
void TreeState::removeAndSaveOrphans()
{
    QList<TreeNode*> children = selected->getChildren();
    TreeNode* parent = selected->getParent();

    parent->addAll(children);

    // Delete the selected node
    //selected->remove();
    delete selected;
}

/* Remove the selected node and any of its children */
void TreeState::removeAndBurnTheOrphanage()
{
    TreeNode* parent = selected->getParent();
    //selected->remove();
    delete selected;
    selected = parent;
}

/* Surround with cut */
void TreeState::surroundWithCut()
{
    // Roots and placeholders cannot be surrounded
    if (selected->isRoot() || selected->isPlaceHolder())
        return;

    // Otherwise, get the old parent
    TreeNode* oldParent = selected->getParent();
    TreeNode* newCut = oldParent->addChildCut();

    // Move the selection into the new cut
    move(selected,newCut);
}

/* Surround with double cut */
void TreeState::surroundWithDoubleCut()
{
    surroundWithCut();
    surroundWithCut();
}

/* Move target to the new parent */
void TreeState::move(TreeNode *target, TreeNode *targetParent)
{
    // Make sure the target is movable
    if (target->isRoot())
        return;

    // Move actually needs a new location
    if (target->parent == targetParent)
        return;

    // Placeholders not allowed to move at this time (too messy)
    if (target->isPlaceHolder())
        return;

    // Verify parent can house the node
    if (targetParent->isStatement())
        return;

    // Parent node as placeholder will be messy, so forbid it for now
    if (targetParent->isPlaceHolder())
        return;

    /* Should be okay to proceed with the move */

    // Remove the target from its old parent
    target->parent->children.removeOne(target);

    // Append it to the new parent's children
    targetParent->children.append(target);

    // Update the target's parent
    target->parent = targetParent;
}

/* Modification mode commands */

/*
 * Removes a double cut from around the current selection, if it exists.
 *
 * TODO: make it a little bit more intelligent by allowing the removal to
 * "fuzzy detect" an acceptable region. So you don't have to exactly select the
 * node itself that the double cut surrounds, you can select maybe one of the
 * cuts themselves and it will figure out what you want to do automatically.
 *
 * Not sure if the above TODO is necessary, but it might be a quality of life
 * addition later on.
 */
void TreeState::doubleCutRemoval()
{

}

/*
 * Adds a double cut around the current selection. This will work against every
 * selected node type except for root nodes and placeholders.
 */
void TreeState::doubleCutAddition()
{

}

/*
 * Sets the subgraph that will be used in iterations and deiterations. This will
 * store a node pointer to be compared against when performIteration and
 * performDeiteration are called afterwards.
 */
void TreeState::setIterationTarget()
{

}

/*
 * Takes the current iteration target and copies it into the selected region if
 * allowed to do so.
 */
void TreeState::performIteration()
{

}

/*
 * Determines if the selection is equivalent to the current iteration target. If
 * it is, remove the selection from the graph.
 */
void TreeState::performDeiteration()
{

}

/* Box print */
QString TreeState::getBoxedString()
{
    // First determine the width of inner region of box
    int width = root->getBoxWidth(0);

    // Header (first row)
    QString result = "<pre>┌";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┐\n";

    // Add each boxLine recursively
    result += root->getBoxLine(0,width,true,"",selected);

    // Footer (last row)
    result += "└";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┘</pre>";

    return result;
}


/* Redraw the tree after changes */
void TreeState::redraw()
{
    emit treeChanged(getBoxedString());
}
