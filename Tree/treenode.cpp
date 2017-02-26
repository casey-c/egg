#include "Tree/treenode.h"
#include "Tree/treestate.h"
#include <algorithm>

#include <QDebug>

/* Debug id */
int TreeNode::globalID = 0;

/* Constructor */
TreeNode::TreeNode()
{
    type = constants::ELEMENT_ROOT;
    name = QString("Root");
    placeHolderChild = false;
    placeholder = NULL;
    parent = NULL;

    myID = globalID++;

}

/* Copy constructor */
TreeNode::TreeNode(TreeNode *original):
    type(original->getType()),
    parent(),
    children(),
    name(original->getName()),
    placeholder(),
    placeHolderChild(),
    myID(globalID++)

{
    if(!original->children.isEmpty())
        for (auto child : original->getChildren())
            this->children.append(TreeNode::copyChildren(child, this));
}

/* Add child cut */
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

/* Add child statement */
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

    // We should be good to add a new child placeholder
    TreeNode* newPlaceholder =
            new TreeNode(constants::ELEMENT_PLACEHOLDER,this,NULL);
    placeHolderChild = true;
    placeholder = newPlaceholder;

    return newPlaceholder;
}

/* Recursively copy a node with their children */
TreeNode* TreeNode::copyChildren(TreeNode* original, TreeNode* parent)
{
    TreeNode* newNode = new TreeNode(original->getType(),parent,
                                     original->getName());
    /* If there is no more children to copy then return the empty node */
    if(original->children.isEmpty())
        return newNode;
    else
        for (auto child : original->getChildren())
            newNode->children.append(TreeNode::copyChildren(child, newNode));

    return newNode;
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


/* Add an existing node as a child */
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

void TreeNode::print(QString indent, bool last)
{
    QString line;
    line += indent;
    if (last)
    {
        line += "└─ ";
        indent += "  ";
    }
    else
    {
        line += "├─ ";
        indent += "│ ";
    }

    if (isRoot())
        qDebug() << QString(line + "Root");
    else
        qDebug() << QString(line + name);

    for (auto child : children)
    {
        child->print(indent, true);
    }
}

QString TreeNode::getFormattedString(QString indent, bool last)
{
    QString line = indent;
    if (last)
    {
        line += "└─ ";
        indent += "  ";
    }
    else
    {
        line += "├─ ";
        indent += "│ ";
    }

    if (isRoot())
        line += "Root\n";
    else if (isStatement())
        line += name + "\n";
    else if (isCut())
        line += "Cut\n";
    else if (isPlaceHolder())
        line += "{?}\n";

    for (auto child : children)
        line += child->getFormattedString(indent,true) + "\n";

    return line;
}

/* Returns a QString to identify this node by type */
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

/* For determining how large the box should be */
int TreeNode::getBoxWidth(int depth)
{
    int myRowLength = (depth * 3) + getTypeID().size();

    int childLength = 0;
    for (auto child : children)
        childLength = std::max(child->getBoxWidth(depth+1),
                               childLength);

    return std::max(myRowLength,childLength);
}

/* For getting this node's line
 * Params:
 *      depth:  how many parents you need to traverse until root
 *      end:    where the line should end (calculated by getBoxWidth beforehand)
 *      bottom: if this node is the last child of our parent (this is for
 *              differentiating between └ and ├ characters when displaying the
 *              tree)
 *      skips:  an int specifying how many │ characters are ignored (WIP)
 */
QString TreeNode::getBoxLine(int depth, int end, bool bottom, QString skips, TreeNode* selected)
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
        bool childIsBottom = (i == children.size() - 1);
        TreeNode* child = children.at(i);
        QString childRow = child->getBoxLine(depth + 1, end, childIsBottom, skips,selected);
        childRows.append(childRow);
    }

    // Combine all the child rows here and append them to result
    for (auto s : childRows)
        result += s;

    return result;
}
