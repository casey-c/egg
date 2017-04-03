#include "Tree/treestate.h"
#include <QDebug>

TreeState::~TreeState()
{
    qDebug() << "Deleting this tree state";
    delete root;
    qDebug() << "Successfully deleted root";
}

bool TreeState::isEqualWith(TreeState* tree)
{
    return this->getRoot()->isEqualWith(tree->getRoot());
}

/////////////////
/// Highlight ///
/////////////////

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

/////////////////
/// Selection ///
/////////////////

/*
 * Adds a node to the selection. Selected nodes have some restrictions:
 *
 * All nodes in the selection list must be siblings of each other (they must
 * have the same parent). This makes sure all selected nodes can be properly
 * iterated if needed.
 *
 * If the parameter node is not a sibling of the existing selectionList, then
 * the selectionList is cleared and updated to include only the newly added
 * node.
 *
 * Params:
 *      node: the node to be added to the selection.
 */
void TreeState::selectSpecific(TreeNode* node)
{
    // In any of these cases, the new node will replace the old selection
    if (     selectionList.isEmpty() ||
             selectionList.first()->isRoot() ||
             node->isRoot() ||
             node->getParent() != selectionList.first()->getParent())
        clearSelection();

    selectionList.append(node);
    return;
}


/*
 * Adds the highlighted node to the current selection list
 */
void TreeState::selectHighlighted()
{
    selectSpecific(highlighted);
}

/*
 * Selects all the children of the highlighted node, if they exist
 *
 * Params:
 *      the node whose children we wish to select
 */
void TreeState::selectChildrenOfHighlighted()
{
    clearSelection();

    for (TreeNode* child : highlighted->getChildren())
        selectSpecific(child);
}

/*
 * Clears all selected nodes
 */
void TreeState::clearSelection()
{
    prevSelection.clear();
    prevSelection = selectionList;

    selectionList.clear();
}

/*
 * Deselects a specific node, if selected
 *
 * Params:
 *      node: the node to remove selection from
 */
void TreeState::deselectNode(TreeNode *node)
{
    selectionList.removeOne(node);
}

/*
 * Deselects the highlighted node, if selected
 */
void TreeState::deselectHighlighted()
{
    deselectNode(highlighted);
}

/*
 * Reverts the selection list to the last set before clear
 */
void TreeState::revertSelectionList()
{
    QList<TreeNode*> list = selectionList;
    selectionList.clear();

    for (TreeNode* node : prevSelection)
        if (!node->isDetached())
            selectSpecific(node);
            //selectionList.append(node);

    prevSelection.clear();
    prevSelection = list;
}

/////////////////////
/// Add functions ///
/////////////////////

/*
 * Adds a child cut to all nodes in the selection list. After doing so, the
 * current selection is cleared but the highlighted node remains untouched.
 */
void TreeState::addChildCut()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // Add child cuts to all selected nodes
    for (TreeNode* node : selectionList)
        recentUpdatedNodes.append(node->addChildCut());

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && !recentUpdatedNodes.isEmpty())
        highlighted = recentUpdatedNodes.first();

    clearSelection();
}

/*
 * Adds a double cut child to all nodes in the selection list. After doing so,
 * the current selection is cleared but the highlighted node remains untouched.
 */
void TreeState::addChildDoubleCut()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    QList<TreeNode*> outerCuts;

    // Add the outer cuts
    for (TreeNode* node : selectionList)
    {
        TreeNode* outerCut = node->addChildCut();
        outerCuts.append(outerCut);
        recentUpdatedNodes.append(outerCut);
    }

    // Add the inner cuts
    TreeNode* target = NULL;
    for (TreeNode* node : outerCuts)
        (target == NULL) ? target = node->addChildCut() : node->addChildCut();

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && target != NULL)
        highlighted = target;

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
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // Add child statements to all selected nodes
    for (TreeNode* node : selectionList)
        recentUpdatedNodes.append(node->addChildStatement(s));

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && !recentUpdatedNodes.isEmpty())
        highlighted = recentUpdatedNodes.first();

    clearSelection();
}

/*
 * Adds an or template to all nodes in the selection list. After doing so, the
 * selection list is cleared.
 */
void TreeState::addOrTemplate()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // For updating highlighted region if solo-selected
    TreeNode* firstInner;

    // Add the or template to all selected nodes
    for (TreeNode* node : selectionList)
    {
        TreeNode* outerCut = node->addChildCut();
        recentUpdatedNodes.append(outerCut);

        // Inner cuts
        firstInner = outerCut->addChildCut();
        outerCut->addChildCut();
    }

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && firstInner != NULL)
        highlighted = firstInner;

    clearSelection();
}

/*
 * Adds a conditional template to all nodes in the selection list. After doing
 * so, the selection list is cleared.
 */
void TreeState::addConditionalTemplate()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // For updating highlighted region if solo-selected
    TreeNode* placeholder;

    // Add a conditional template to all selected nodes
    for (TreeNode* node : selectionList)
    {
        TreeNode* outer = node->addChildCut();
        recentUpdatedNodes.append(outer);

        // Inner cut and placeholder
        outer->addChildCut();
        placeholder = outer->addChildPlaceholder();
    }

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && placeholder != NULL)
        highlighted = placeholder;

    clearSelection();
}

/*
 * Adds a biconditional template to all nodes in the selection list. After doing
 * so, the selection list is cleared.
 */
void TreeState::addBiconditionalTemplate()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // For updating highlighted region if solo-selected
    TreeNode* placeholder;

    // Add biconditional template to all selected nodes
    for (TreeNode* node : selectionList)
    {
        TreeNode* first = node->addChildCut();
        TreeNode* second = node->addChildCut();

        recentUpdatedNodes.append(first);
        recentUpdatedNodes.append(second);

        first->addChildCut();
        placeholder = first->addChildPlaceholder();

        second->addChildCut();
        second->addChildPlaceholder();
    }

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && placeholder != NULL)
        highlighted = placeholder;

    clearSelection();
}

////////////////////////
/// Remove functions ///
////////////////////////

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

/*
 * Detaches the target node. It doesn't delete it from memory, but stores it
 * inside a command somewhere. This is the function that gets called on a remove
 * command so that we only have to remember a single node pointer instead of an
 * entire copied tree state. (Making the execute() function only need to detach
 * a node while the undo() function only needs to reattach)
 *
 * Params:
 *      target: the node that will be detached
 */
void TreeState::detachNode(TreeNode *target)
{
    // Root nodes cannot be detached
    if (target->isRoot())
        return;

    // Placeholders cannot be detached (for now)
    // TODO: add placeholder logic too
    if (target->isPlaceHolder())
        return;

    // Remember the old parent
    TreeNode* parent = target->getParent();
    highlightSpecific(parent);

    // Detach the parent from the child
    TreeNode::detach(target, parent);

    recentUpdatedNodes.append(target);
    recentParents.append(parent);
}

/*
 * Calls detach on all the selected nodes (or highlighted if none selected)
 *
 * Clears the selection list after completing its operation
 */
void TreeState::detachNodes()
{
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    for (TreeNode* node : selectionList)
        detachNode(node);

    clearSelection();
}

/*
 * Detaches the current node, but moves all its children to their grandparent.
 *
 * Upon completion, the selection list is cleared.
 */
void TreeState::detachNodeAndMoveOrphans()
{
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    for (TreeNode* node : selectionList)
    {
        // Root nodes cannot be detached
        if (node->isRoot())
            break;

        // Placeholder nodes cannot be detached
        // TODO: add logic
        if (node->isPlaceHolder())
            break;

        TreeNode* parent = node->getParent();

        // Paranoid null check
        if (parent == NULL)
            continue;


        QList<TreeNode*> children;

        // Move the children
        for (TreeNode* child : node->getChildren())
        {
            TreeNode::move(child,parent);
            children.append(child);
        }

        // Remember these altered children
        recentListsOfChildren.append(children);

        // Now that the children are taken care of, detach the childless node
        detachNode(node);
    }

    clearSelection();

}

//////////////////////////
/// Surround functions ///
//////////////////////////

/*
 * Surrounds each selected node with a cut, if possible. Root nodes cannot be
 * surrounded. After completing this operation, the selection list is cleared.
 */
void TreeState::surroundWithCut()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // Surround all selected nodes with cuts
    for (TreeNode* node : selectionList)
    {
        // Roots cannot be surrounded
        if (node->isRoot())
            return;

        TreeNode* oldParent = node->getParent();
        TreeNode* newCut = oldParent->addChildCut();
        TreeNode::move(node,newCut);

        recentUpdatedNodes.append(newCut);
    }

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && !recentUpdatedNodes.isEmpty())
        highlighted = recentUpdatedNodes.first();

    clearSelection();
}

/*
 * Surrounds each selected node with a double cut, if possible. Roots cannot be
 * surrounded. After completing this operation, the selection is cleared.
 */
void TreeState::surroundWithDoubleCut()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // Surround all selected nodes with double cuts
    for (TreeNode* node : selectionList)
    {
        // Nodes cannot be surrounded
        if (node->isRoot())
            return;

        TreeNode* oldParent = node->getParent();
        TreeNode* newOuterCut = oldParent->addChildCut();
        TreeNode* newInnerCut = newOuterCut->addChildCut();

        TreeNode::move(node,newInnerCut);
    }

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && !recentUpdatedNodes.isEmpty())
        highlighted = recentUpdatedNodes.first();

    clearSelection();
}

/*
 * Surrounds the selected nodes with a single cut. All selected nodes that
 * are siblings remain siblings, and the selection is cleared afterwards
 */
void TreeState::surroundWithCutAsGroup()
{
    // With no selection, act on the highlighted node
    if (selectionList.isEmpty())
        selectionList.append(highlighted);

    // Roots cannot be surrounded
    if (selectionList.first()->isRoot())
        return;

    // Add the cut to the proper node
    TreeNode* parent = selectionList.first()->getParent();
    TreeNode* cut = parent->addChildCut();

    recentUpdatedNodes.append(cut);

    // Move all the selected nodes into the new cut
    for (TreeNode* node : selectionList)
        TreeNode::move(node, cut);

    // If we acted on the highlighted node, update the highlight
    if (selectionList.size() == 1 && !recentUpdatedNodes.isEmpty())
        highlighted = recentUpdatedNodes.first();

    clearSelection();
}

////////////
/// Move ///
////////////

/*
 * Moves the target node to a different parent.
 *
 * Params:
 *      target: the node whose parent we wish to update.
 *      targetParent: the new parent we want the node to have
 */
void TreeState::move(TreeNode *target, TreeNode *targetParent)
{
    TreeNode::move(target,targetParent);
}

/////////////////////////
/// Modification mode ///
/////////////////////////

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
    /*
    // First, check if the selection has a double cut surrounding it
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
    */
    return NULL;
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
    /*
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
    */
    return NULL;
}

/*
 * Makes the current iteration list the selection list. Doing so will clear the
 * selection list.
 *
 * TODO: this has a potential for bugs if the nodes saved in the iteration list
 * are deleted before the iterationList is used/cleared. Ideally, the iteration
 * list must be updated any time a remove() is called on a node, just in case
 * the node being deleted is also in this iterationList - we don't want a bad
 * pointer to deleted memory to stick around and used in iterations/deiterations
 * later.
 */
void TreeState::setIterationTarget()
{
    iterationList = selectionList;
}

/*
 * Takes the current iteration target and copies it into the selected region if
 * allowed to do so.
 *
 * TODO: test this code (I don't think it works as intended)
 */
TreeState* TreeState::performIteration()
{
    /*
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
    */
    return NULL;
}


/*
 * Determines if the selection is equivalent to the current iteration target. If
 * it is, remove the selection from the graph.
 */
TreeState* TreeState::performDeiteration()
{
    // Determine if the selection is logically equivalent to the target
    // TODO: implement an .equals() to compare TreeNodes
    return NULL;
}

/////////////////
/// Box print ///
/////////////////

/*
 * Prints out the tree inside a box. (For sending to the TreeDisplayWidget)
 */
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
    result += root->getBoxLine(0, width, true,
                               "", highlighted, selectionList);

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


/*
 * Returns and clears the most recent added nodes. This is used as a helper for
 * commands. For example, the command will call an add() function on all the
 * selected nodes in the tree. For all nodes that are affected, a new node may
 * be created. These new nodes will appear in the recentAddedNodes list.
 *
 * The command then calls this pop() function to determine which nodes were
 * added, so it knows which nodes to delete if an undo() is ever called on it.
 *
 * TODO: buggy behavior with placeholders. Commands that replace a placeholder
 * with say a cut or something will not remake the placeholder after an undo
 * call, it will delete the promoted placeholder node. A potential fix may be
 * to set a flag in TreeNode to say if it is a promoted placeholder, and then
 * the command.undo() will check for this flag. If found, the command is
 * responsible for adding a placeholder to the parent after the node itself is
 * deleted.
 */
QList<TreeNode*> TreeState::popRecentNodes()
{
    QList<TreeNode*> list = recentUpdatedNodes;
    recentUpdatedNodes.clear();
    return list;
}

QList<TreeNode*> TreeState::popRecentParents()
{
    QList<TreeNode*> list = recentParents;
    recentParents.clear();
    return list;
}

QList< QList<TreeNode*> > TreeState::popRecentChildren()
{
    QList< QList<TreeNode*> > list = recentListsOfChildren;
    recentListsOfChildren.clear();
    return list;
}

//////////////
/// Pounce ///
//////////////

/*
 * Sets all the pounce ID's using DFS
 */
void TreeState::setPounceIDs()
{
    QStack<TreeNode*> stack;

    stack.push(root);

    int id = 0;

    while (!stack.isEmpty())
    {
        TreeNode* node = stack.pop();

        // Add the ID here
        node->setPounceID(id++);

        for (TreeNode* child : node->getChildren())
            stack.push(child);

    }
}

/*
 * Draws the tree as pounce values
 */
void TreeState::drawPounceTree()
{
    emit treeChanged(getPounceString());
}

/*
 * Builds the pounce string as a box
 */
QString TreeState::getPounceString()
{
    // First determine the width of inner region of box
    int width = root->getPounceWidth(0);

    // Header (first row)
    QString result = "<pre>┌";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┐\n";

    // Add each boxLine recursively
    result += root->getPounceLine(0, width, true,
                               "", highlighted, selectionList);

    // Footer (last row)
    result += "└";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┘</pre>";

    return result;
}

/*
 * Attempts to highlight the node associated with the pounce string. If it
 * exists, highlight that node. If it doesn't, do nothing.
 *
 * Params:
 *      target: the string we're looking for as a pounceID of the nodes
 */
void TreeState::pounceTo(QString target)
{
    // DFS for the node
    QStack<TreeNode*> stack;
    stack.push(root);

    while (!stack.isEmpty())
    {
        TreeNode* node = stack.pop();
        if (QString::compare(target,node->getPounceID()) == 0)
        {
            // Found our guy
            highlightSpecific(node);
            return;
        }

        // Otherwise, keep searching
        for (TreeNode* child : node->getChildren())
            stack.push(child);
    }

    // We couldn't find the node, oh well
}


