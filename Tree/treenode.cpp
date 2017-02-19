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
