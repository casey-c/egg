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
 * Adds a child cut to all nodes in the selection list. After doing so, the
 * current selection is cleared but the highlighted node remains untouched.
 */
void TreeState::addChildCut()
{
    for (TreeNode* node : selectionList)
        node->addChildCut();

    clearSelection();
}

/*
 * Adds a double cut child to all nodes in the selection list. After doing so,
 * the current selection is cleared but the highlighted node remains untouched.
 */
void TreeState::addChildDoubleCut()
{
    QList<TreeNode*> outerCuts;

    // Add the outer cuts
    for (TreeNode* node : selectionList)
        outerCuts.append(node->addChildCut());

    // Add the inner cuts
    for (TreeNode* node : outerCuts)
        node->addChildCut();

    clearSelection();
}

/*
 * Adds a child statement to all nodes in the selection list. After doing so,
 * the selection list is cleared but the highlighted node remains untouched.
 *
 * Params:
 *      s: the string representation of the statement
 */
void TreeState::addChildStatement(QString s)
{
    for (TreeNode* node : selectionList)
        node->addChildStatement(s);

    clearSelection();
}

/* Adds an or template */
bool TreeState::addOrTemplate()
{
    bool anyAdded = false;
    for (TreeNode* node : selectionList)
    {
        // Statements & placeholders are forbidden from adding templates
        if (node->isStatement())
            continue;
        else
            anyAdded = true;

        // Add the actual template
        node->addChildCut();

    }
}

/*
 * Adds an or template to all nodes in the selection list. After doing so, the
 * selection list is cleared.
 */
void TreeState::addOrTemplate()
{
    for (TreeNode* node : selectionList)
    {
        TreeNode* outerCut = node->addChildCut();
        outerCut->addChildCut();
        outerCut->addChildCut();
    }

    clearSelection();
}

/*
 * Adds a conditional template to all nodes in the selection list. After doing
 * so, the selection list is cleared.
 */
void TreeState::addConditionalTemplate()
{
    for (TreeNode* node : selectionList)
    {
        TreeNode* outer = node->addChildCut();
        outer->addChildCut();
        outer->addChildPlaceholder();
    }

    clearSelection();
}

/*
 * Adds a biconditional template to all nodes in the selection list. After doing
 * so, the selection list is cleared.
 */
void TreeState::addBiconditionalTemplate()
{
    for (TreeNode* node : selectionList)
    {
        TreeNode* first = node->addChildCut();
        TreeNode* second = node->addChildCut();

        first->addChildCut();
        first->addChildPlaceholder();

        second->addChildCut();
        second->addChildPlaceholder();
    }

    clearSelection();
}

/*
 * Removes the target node, but transfers ownership of all its children to their
 * grandparent. Saving the orphans means we won't delete any kids that the node
 * has, but instead try to move them.
 *
 * Placeholder elements are always deleted and remade if neccessary. The call to
 * parent->addChildPlaceholder() will not make a second placeholder if the
 * parent already has one, as that function will enforce the one placeholder per
 * node limit.
 *
 * Root elements cannot be removed through this function; they are taken care of
 * only in the destructor.
 *
 * Params:
 *      target: the node to be removed (its children are safely moved)
 */
void TreeState::removeAndSaveOrphans(TreeNode *target)
{
    // Root cannot be removed
    if (target->isRoot())
        return;

    // The node who inherits the orphans
    TreeNode* parent = target->getParent();

    // Placeholder logic
    if (target->hasPlaceHolder())
        parent->addChildPlaceholder();

    // Save the children
    for (TreeNode* orphan : target->getChildren())
        TreeNode::move(orphan,parent);

    // Update selection and highlight info if necessary
    selectionList.removeOne(target);
    if (highlighted == target)
        highlighted = parent;

    delete target;
}

/*
 * Removes the target node, crashes the plane, and leaves no survivors (all
 * children of the target node are lost).
 *
 * Params:
 *      target: the node to be removed, and all its children lost
 */
void TreeState::removeAndBurnTheOrphanage(TreeNode *target)
{
    // Roots can't be removed (they are taken care of by the destructor)
    if (target->isRoot())
        return;

    // Update selection and highlight info if necessary
    selectionList.removeOne(target);
    if (highlighted == target)
        highlighted = target->getParent();

    // They expect one of us in the wreckage, brother
    delete target;
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
