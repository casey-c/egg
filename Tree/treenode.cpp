#include "Tree/treenode.h"
#include "Tree/treestate.h"

/* Add child cut */
TreeNode* TreeNode::addChildCut()
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_CUT;
        parent->placeHolderChild = false;
        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_CUT;
        placeHolderChild = false;

        return placeholder;
    }

    // Otherwise, make a new cut element
    TreeNode* newCut = new TreeNode(constants::ELEMENT_CUT,this,NULL);
    children.append(newCut);
    return newCut;
}

/* Add child statement */
TreeNode* TreeNode::addChildStatement(QString s)
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_STATEMENT;
        name = s;
        parent->placeHolderChild = false;

        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_STATEMENT;
        placeholder->name = s;
        placeHolderChild = false;

        return placeholder;
    }

    // Otherwise, make a new statement element
    TreeNode* newStatement = new TreeNode(constants::ELEMENT_STATEMENT,this,s);
    children.append(newStatement);

    return newStatement;

}

/* Add child placeholder */
TreeNode* TreeNode::addChildPlaceholder()
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is already a placeholder
    if (isPlaceHolder())
        return this;

    // Check if we already have a child placeholder
    if (placeHolderChild)
        return this;

    // We should be good to add a new child placeholder
    TreeNode* newPlaceholder = new TreeNode(constants::ELEMENT_PLACEHOLDER,this,NULL);
    placeHolderChild = true;
    placeholder = newPlaceholder;

    return newPlaceholder;
}

/* Add all */
void TreeNode::addAll(QList<TreeNode *> list)
{
    // Check for empty list
    if (list.isEmpty())
        return;

    // Check if this is allowed to have children
    if (isStatement())
        return;

    // This node is a placeholder
    if (isPlaceHolder())
    {
        TreeNode* first = list.first();
        list.removeFirst();

        // Replace this with first node in the list
        this->type = first->type;
        this->name = first->name;

        // Add the rest as siblings to this node
        for (TreeNode* node : list)
            parent->addExisting(node);

        // Finished
        return;
    }

    // This is a parent of a placeholder
    else if (placeHolderChild)
    {
        // Replace the placeholder child with the first item in the list
        TreeNode* first = list.first();
        list.removeFirst();

        placeholder->type = first->type;
        placeholder->name = first->name;

        placeHolderChild = false;

        // Then add the other items as additional children
        for (TreeNode* node : list)
            this->addExisting(node);
    }

}


/* Add an existing node as a child */
void TreeNode::addExisting(TreeNode *node)
{
    // Check to make sure the existing node doesn't conflict with placeholder
    // restrictions
    if (node->isPlaceHolder() && this->placeHolderChild)
        return;

    // Otherwise, we should be free to add a new child
    TreeNode* newNode = new TreeNode(node->type,this,node->name);
    children.append(newNode);
}

/*
 * Remove this node from the tree
 *
 * Warning: removing a node will DELETE it from the heap
 */
void TreeNode::remove()
{
    // Remove us from our parent's list of children
    parent->children.removeOne(this);

    // Delete us permanently
    delete this;
}
