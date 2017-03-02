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

/* Remove the node, but give its children to their grandparent */
void TreeState::removeAndSaveOrphans()
{
    // Undefined behavior
    if (selected->isRoot() || selected->isPlaceHolder())
        return;

    // The grandparent who will inherit the orphans
    TreeNode* grandparent = selected->getParent();

    // Move the children
    for (auto child : selected->getChildren())
        TreeNode::move(child,grandparent);

    // If the selected has a placeholder, but the grandparent doesn't yet
    if (selected->hasPlaceHolder() && !grandparent->hasPlaceHolder())
        grandparent->addChildPlaceholder();

    // Delete and update the selected region
    delete selected;
    selected = grandparent;
}

/* Remove the selected node and any of its children */
void TreeState::removeAndBurnTheOrphanage()
{
    TreeNode* parent = selected->getParent();
    delete selected;
    selected = parent;
}

/* Surround with cut */
TreeNode* TreeState::surroundWithCut()
{
    // Roots and placeholders cannot be surrounded
    if (selected->isRoot() || selected->isPlaceHolder())
        return NULL;

    // Otherwise, get the old parent
    TreeNode* oldParent = selected->getParent();
    TreeNode* newCut = oldParent->addChildCut();

    // Move the selection into the new cut
    move(selected,newCut);

    return newCut;
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
    TreeNode::move(target,targetParent);
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
TreeState* TreeState::doubleCutRemoval()
{
    /* First, check if the selection has a double cut surrounding it */
    // Root and placeholders are disallowed
    if (selected->isRoot() || selected->isPlaceHolder())
    {
        qDebug() << "ERROR: function not allowed for root and placeholders";
        return this;
    }

    // Parent must be a cut
    TreeNode* parent = selected->getParent();
    if (!parent->isCut())
    {
        qDebug() << "ERROR: parent not cut";
        return this;
    }

    // Grandparent must be a cut.
    TreeNode* grandparent = parent->getParent();
    if (!grandparent->isCut())
    {
        qDebug() << "ERROR: grandparent not cut";
        return this;
    }

    // The selected node is surrounded by a double cut, but make sure the parent
    // doesn't have any siblings
    if (grandparent->getChildren().size() != 1)
    {
        qDebug() << "ERROR: outer cut must only contain the inner cut";
        return this;
    }

    // Copy the tree state
    TreeState* newState = new TreeState(this);

    // Determine what nodes need changing
    TreeNode* newSelection = newState->getSelected();
    TreeNode* newParent = newSelection->getParent();
    TreeNode* newGrandparent = newParent->getParent();

    // Delete the double cuts, but update the children
    newState->selectSpecific(newParent);
    newState->removeAndSaveOrphans();
    newState->selectSpecific(newGrandparent);
    newState->removeAndSaveOrphans();

    // Return the new state with the double cut removed
    return newState;
}

/*
 * Adds a double cut around the current selection. This will work against every
 * selected node type except for root nodes and placeholders.
 *
 * TODO: this function really needs multiple selection support. We need to
 * rework how selection is handled so we can surround a set of statements with a
 * double cut. As it is now, only a single statement can be the target, and that
 * is not good / what we want.
 */
TreeState* TreeState::doubleCutAddition()
{
    // Verify that the selection can be surrounded with a double cut
    if (selected->isRoot())
    {
        qDebug() << "ERROR: can't surround the root with a double cut";
        return this;
    }

    // Make a new tree state
    TreeState* newState = new TreeState(this);

    // Perform the addition
    TreeNode* newSelected = newState->getSelected();
    TreeNode* parent = newSelected->getParent();

    TreeNode* newOuterCut = parent->addChildCut();
    TreeNode* newInnerCut = newOuterCut->addChildCut();

    // Perform the move
    TreeNode::move(newSelected,newInnerCut);

    // Return the new state with the added double cut
    return newState;
}

/*
 * Sets the subgraph that will be used in iterations and deiterations. This will
 * store a node pointer to be compared against when performIteration and
 * performDeiteration are called afterwards.
 *
 * TODO: multiple selection, right now only a single node can be the iteration
 * target. We need to be able to select multiple nodes and iterate all of them
 * at once.
 */
void TreeState::setIterationTarget()
{
    iterationTarget = selected;
}

/*
 * Takes the current iteration target and copies it into the selected region if
 * allowed to do so.
 *
 * TODO: test this code (I don't think it works as intended)
 */
TreeState* TreeState::performIteration()
{
    // First, verify if the new selection is a valid space to iterate into
    if (selected->isPlaceHolder() || selected->isStatement())
        return this;

    // Make sure the target is some child of the selected region
    TreeNode* node = selected;
    bool valid = false;

    while (node != NULL)
    {
        if (node == iterationTarget)
        {
            valid = true;
            break;
        }

        node = node->getParent();
    }

    // Unable to find the iteration target as an ancestor to the movable node
    if (!valid)
    {
        qDebug() << "ERROR: invalid iteration target";
        return this;
    }

    // Otherwise, we should be able to copy the treestate and do the iteration
    TreeState* newState = new TreeState(this);

    // Perform the copy
    TreeNode* newSelected = newState->getSelected();
    TreeNode* newTarget = newState->getIterationTarget();

    newSelected->addExistingByCopy(newTarget);

    return newState;
}


/*
 * Determines if the selection is equivalent to the current iteration target. If
 * it is, remove the selection from the graph.
 */
TreeState* TreeState::performDeiteration()
{
    // Determine if the selection is logically equivalent to the target
    // TODO: implement an .equals() to compare TreeNodes
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
