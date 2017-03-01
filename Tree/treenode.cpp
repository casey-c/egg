#include "Tree/treenode.h"
#include "Tree/treestate.h"
#include <algorithm>

#include <QDebug>

/* Debug id */
int TreeNode::globalID = 0;

/* Copy constructor */
TreeNode::TreeNode(TreeNode *original):
    type(original->getType()),
    parent(),
    name(original->getName()),
    placeHolderChild(false),
    myID(globalID++),
    children(),
    placeholder()

{
    if(original->hasPlaceHolder()){
        placeHolderChild = true;
        placeholder = TreeNode::copyChildren(original->getPlaceHolder(),this);
    }
    for (auto child : original->getChildren())
        this->children.append(TreeNode::copyChildren(child, this));
}

/*
 * Destructor will free up any memory allocated for this node
 */
TreeNode::~TreeNode()
{
    // Remove us from our parent's list of children
    if (!isRoot())
        parent->children.removeOne(this);

    // Remove our children
    for (auto child : children)
        delete child;

    // Remove a placeholder
    if (placeHolderChild)
        delete placeholder;
}

/* Removes a child without deleting it
 *
 * TODO: fix this to be less dangerous!
 */
void TreeNode::removeAChildWithoutDelete(TreeNode *child)
{
    qDebug() << "Originally children size was "<<children.size();
    children.removeOne(child);

    qDebug() << "New children size is "<<children.size();
}

/* Sets the parent
 *
 * TODO: fix this to be less dangerous!
 */
void TreeNode::setParent(TreeNode *newParent)
{
    parent = newParent;
}

/*
 * Adds a child cut to this node
 */
TreeNode* TreeNode::addChildCut()
{
    // Add as sibling instead of instantly returning
    if (isStatement())
        return parent->addChildCut();

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_CUT;

        // Update our parent's info
        parent->placeHolderChild = false;
        parent->children.append(this);
        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_CUT;
        placeHolderChild = false;

        // Make it our real child
        children.append(placeholder);
        return placeholder;
    }

    // Otherwise, make a new cut element
    TreeNode* newCut = new TreeNode(constants::ELEMENT_CUT,this,NULL);
    children.append(newCut);
    return newCut;
}

/*
 * Adds a child statement to this node.
 *
 * Params:
 *      s: the string that describes this statement
 */
TreeNode* TreeNode::addChildStatement(QString s)
{
    // Add as sibling instead of instantly returning
    if (isStatement())
        return parent->addChildStatement(s);

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_STATEMENT;
        name = s;

        // Update our parent's info
        parent->placeHolderChild = false;
        parent->children.append(this);
        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_STATEMENT;
        placeholder->name = s;

        // Make it a real boy
        placeHolderChild = false;
        children.append(placeholder);

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
        return placeholder;

    // Okay to add new placeholder as a child
    TreeNode* newPlaceholder = new TreeNode(constants::ELEMENT_PLACEHOLDER,
                                            this,
                                            NULL);
    placeHolderChild = true;
    placeholder = newPlaceholder;

    return newPlaceholder;
}

/* Recursively copy a node with their children */
TreeNode* TreeNode::copyChildren(TreeNode* original, TreeNode* parent)
{
    // Copy the original node into a new node
    TreeNode* newNode = new TreeNode(original->getType(),
                                     parent,
                                     original->getName());

    // Recurse on the original's children
    for (auto child : original->getChildren())
        newNode->children.append(TreeNode::copyChildren(child, newNode));

    return newNode;
}

/*
 * Adds all the TreeNodes in list as children of our own
 *
 * Params:
 *      list: pointers to the nodes to add
 */
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

        // Make sure our parent knows we're a real boy now
        parent->children.append(this);
        parent->placeHolderChild = false;

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

        // Update parent info
        placeHolderChild = false;
        children.append(placeholder);

        // Then add the other items as additional children
        for (TreeNode* node : list)
            this->addExisting(node);
    }

}

/*
 * Copies the node and adds the copy as a child to this one.
 *
 * Params:
 *      node: a pointer to the node that needs to be copied
 */
void TreeNode::addExisting(TreeNode *node)
{
    // Check to make sure the existing node doesn't conflict with placeholder
    // restrictions
    if (node->isPlaceHolder())
    {
        // Only allowed one placeholder per parent node
        if (this->placeHolderChild)
            return;

        // Otherwise, set this node as our new placeholder
        placeHolderChild = true;
        placeholder = node;
        return;
    }

    // Otherwise, we should be free to add a new child
    TreeNode* newNode = new TreeNode(node->type,this,node->name);
    children.append(newNode);
}

/*
 * Appends the existing node to this node's children
 */
void TreeNode::appendExistingToChildren(TreeNode *node)
{
    children.append(node);

    // Update the target's parent
    node->setParent(this);
}

/*
 * Changes the type int into a readable QString
 */
QString TreeNode::getTypeID()
{
    if (isRoot())
        return QString("Root");
    else if (isStatement())
        return name;
    else if (isCut())
        return QString("Cut");

    return QString("{?}");
}

/*
 * Returns the total width of this line, for determining how wide to make the
 * box
 *
 * Params:
 *      depth: how many parents you need to traverse until root
 */
int TreeNode::getBoxWidth(int depth)
{
    int myRowLength = (depth * 3) + getTypeID().size();

    int childLength = 0;
    for (auto child : children)
        childLength = std::max(child->getBoxWidth(depth+1),
                               childLength);

    return std::max(myRowLength,childLength);
}

/*
 * Returns a correctly formatted QString of this node's line in the box-drawing
 * form.
 *
 * Params:
 *      depth:  how many parents you need to traverse until root
 *      end:    where the line should end (calculated by getBoxWidth beforehand)
 *      bottom: if this node is the last child of our parent (this is for
 *              differentiating between └ and ├ characters when displaying the
 *              tree)
 *      skips:  binary string to determine whether to draw a branch upward or
 *              to skip that vertical line. this is inherited from parent, with
 *              an appended 0 or 1 based on whether the parent itself was a
 *              bottom piece, respectively
 *      selected: a pointer to the treestate's selceted node; this determines
 *              when to apply a (*) to a row
 */
QString TreeNode::getBoxLine(int depth,
                             int end,
                             bool bottom,
                             QString skips,
                             TreeNode* selected)
{
    QString result = "│ ";

    // Root does less work
    if (isRoot())
    {
        result += "Root";
        for (int i = 0; i < end - 4; ++i)
            result += " ";
    }
    else // Non-root elements are a tad more complicated
    {
        // Use the skips string to determine whether to leave space or draw
        // vertical line for branches to the left
        for (int i = 0; i < skips.length(); ++i)
        {
            if (skips.at(i) == '0')
                result += "   ";
            else
                result += "│  ";
        }

        // Start of the branch to parent
        if (bottom)
        {
            skips.append('0');
            result += "└──";
        }
        else
        {
            skips.append('1');
            result += "├──";
        }

        // Add the label for this row
        result += getTypeID();

        // Add the remaining space
        int used = (3 * (depth - 1)) + getTypeID().size() + 3;
        for (int i = 0; i < (end - used); ++i )
            result += " ";
    }

    // End of the row
    result += " │";
    if (selected == this)
            result += "(*)";
    result += "\n";

    // Now figure out all the children
    QList<QString> childRows;
    for (int i = 0; i < children.size(); ++i)
    {
        TreeNode* child = children.at(i);
        QString childRow = child->getBoxLine(depth + 1,
                                             end,
                                             i == children.size() - 1,
                                             skips,
                                             selected);
        childRows.append(childRow);
    }

    // Combine all the child rows here and append them to result
    for (auto s : childRows)
        result += s;

    return result;
}
