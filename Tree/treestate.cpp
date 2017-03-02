#include "Tree/treestate.h"
#include <QDebug>

TreeState::~TreeState()
{
    delete root;
}

/*
 * Highlights a child of the current highlighted node, if it exists
 */
void TreeState::highlightChild()
{
    if (!highlighted->getChildren().isEmpty())
            highlightSpecific(highlighted->getChildren().first());
}

/*
 * Highlights the parent of the current highlighted node
 */
void TreeState::highlightParent()
{
    if (!highlighted->isRoot())
        highlightSpecific(highlighted->getParent());
}


/*
 * Highlights the root node
 */
void TreeState::highlightRoot()
{
    highlightSpecific(root);
}

/*
 * Highlights the left sibling of the current highlighted node
 */
void TreeState::highlightLeftSibling()
{
    // Root doesn't have any siblings
    if (highlighted->isRoot())
        return;

    // List contains the highlighted item and all its siblings
    QList<TreeNode*> list = highlighted->getParent()->getChildren();
    TreeNode* previous = NULL;

    // Look for a node to the left
    for (TreeNode* curr : list)
    {
        if (curr == highlighted)
            break;
        previous = curr;
    }

    // Nothing to the left
    if (previous == NULL)
        highlightSpecific(list.last());
    else // Highlight the node to the left
        highlightSpecific(previous);
}

/*
 * Highlights the right sibling of the currently highlighted node
 */
void TreeState::highlightRightSibling()
{
    // Root doesn't have any siblings
    if (highlighted->isRoot())
        return;

    // List contains the highlighted node and all its siblings
    QList<TreeNode*> list = highlighted->getParent()->getChildren();

    TreeNode* next = NULL;
    QList<TreeNode*>::const_reverse_iterator it = list.rbegin();
    for (; it != list.rend(); ++it)
    {
        if (*it == highlighted)
            break;
        next = *it;
    }

    // Nothing to the right
    if (next == NULL)
        highlightSpecific(list.front());
    else // Found something to the right
        highlightSpecific(next);
}

/*
 * Highlights a specific node
 *
 * Params:
 *      node: the node to be highlighted
 */
void TreeState::highlightSpecific(TreeNode* node)
{
    highlighted = node;
}

/*
 * Adds a child cut to all valid nodes in the current selection. If any nodes
 * are successfully added, the selection will be cleared and the highlighted
 * node will remain untouched.
 *
 * Returns:
 *      true: if any child cuts were added
 *      false: otherwise
 */
bool TreeState::addChildCut()
{
    bool anyAdded = false;

    for (TreeNode* node : selectionList)
    {
        if (node->addChildCut())
            anyAdded = true;
    }

    if (!anyAdded)
        return false;

    clearSelection();
    return true;
}

/*
 * Adds a double cut to all valid nodes in the current selection. All double
 * cuts that have been successfully added will have their inner cut selected.
 * The selection list will be cleared if any cuts are added.
 *
 * Returns:
 *      true: if any double cuts are added
 *      false: otherwise
 */
bool TreeState::addChildDoubleCut()
{
    QList<TreeNode*> outerCuts;

    // Add the outer cuts
    for (TreeNode* node : selectionList)
    {
        if (node->addChildCut())
            outerCuts.append(node->getChildren().last());
    }

    // Add the inner cuts
    for (TreeNode* node : outerCuts)
        node->addChildCut();

    // No cuts were added
    if (outerCuts.isEmpty())
        return false;

    // Everything was successful
    clearSelection();
    return true;
}

/*
 * Adds a child statement to all the selected nodes if possible. If any
 * statements are added, the selection is cleared and the function returns true.
 *
 * Params:
 *      s: the string representation of the statement
 *
 * Returns:
 *      true: if at least one statement was successfully added
 *      false: otherwise
 */
bool TreeState::addChildStatement(QString s)
{
    bool anyAdded = false;
    for (TreeNode* node : selectionList)
    {
        if (node->addChildStatement(s))
            anyAdded = true;
    }

    // Nothing added
    if (!anyAdded)
        return false;

    // Everything was successful
    clearSelection();
    return true;
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


//////////////////////////
/// Multiple selection ///
//////////////////////////
/*
 * Clears all selected nodes
 */
void TreeState::clearSelection()
{
    selectionList.clear();
}

/*
 * Adds the highlighted node to selection
 */
void TreeState::addToSelectionList()
{
    // Root nodes replace any previous selection
    if (highlighted->isRoot())
    {
        clearSelection();
        selectionList.append(highlighted);
        return;
    }

    // Check if highlighted object is already in selection list or if it's
    // allowed to join (selections must be siblings)
    for (TreeNode* node : selectionList)
        if (    node == highlighted ||
                node->getParent() != highlighted->getParent())
            return;

    // We should be okay to add the highlighted node
    selectionList.append(highlighted);
}

/*
 * Removes the highlighted node from selection list
 */
void TreeState::removeFromSelectionList()
{
    selectionList.removeOne(highlighted);
}
