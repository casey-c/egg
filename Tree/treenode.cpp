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

/*
 * Adds a child cut to this node. This function will always succeed and return
 * the added node (or promoted placeholder). If this node is a statement, a new
 * cut will be created as a sibling since statements aren't allowed to have
 * children of their own.
 *
 * Returns:
 *      the added cut
 */
TreeNode* TreeNode::addChildCut()
{
    // Add as sibling instead of instantly returning
    if (isStatement())
        return parent->addChildCut();

    // Check if this is a placeholder
    else if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_CUT;

        // Update our parent's info
        parent->placeHolderChild = false;
        parent->children.append(this);
        return this;
    }

    // See if we have a placeholder child to replace
    else if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_CUT;
        placeHolderChild = false;

        // Make it our real child
        children.append(placeholder);
        return placeholder;
    }

    // Otherwise, make a new cut element
    else
    {
        TreeNode* newCut = new TreeNode(constants::ELEMENT_CUT,this,NULL);
        children.append(newCut);
        return newCut;
    }
}

/*
 * Adds a child statement to this node. If this is a statement, it adds it as a
 * sibling to this node. This function will always succeed and return the node
 * it creates or updates.
 *
 * Params:
 *      s: the string that describes this statement
 *
 * Returns:
 *      the added node or promoted placeholder
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
    else if (placeHolderChild)
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
    else
    {
        TreeNode* newStatement = new TreeNode(constants::ELEMENT_STATEMENT,this,s);
        children.append(newStatement);
        return newStatement;
    }
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
            parent->addExistingByCopy(node);

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
            this->addExistingByCopy(node);
    }

}

/*
 * Copies the node and adds the copy as a child to this one.
 *
 * Params:
 *      node: a pointer to the node that needs to be copied
 */
void TreeNode::addExistingByCopy(TreeNode *node)
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
 * Move will update pointers to move target in the tree structure, with a couple
 * restrictions on what types of operations are allowed.
 *
 * Params:
 *      target: the node to move (must not be root or placeholder)
 *      targetParent: what new parent the target needs to have
 *
 * Undefined behavior:
 * Moving a target to one of its children is a dangerous edge case not currently
 * accounted for. This function assumes that it won't happen, and performs no
 * checks to verify that it won't. Target must only be moved either up the tree
 * or on a different branch sideways, never down.
 */
void TreeNode::move(TreeNode *target, TreeNode *targetParent)
{
    // Make sure the target is movable
    if (target->isRoot())
        return;

    // Move actually needs a new location
    if (target->getParent() == targetParent)
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
    if (target->getParent() != NULL)
        target->getParent()->children.removeOne(target);
    targetParent->children.append(target);
    target->parent = targetParent;
}

/*
 * Detaches a node by disconnecting the parent from the child
 */
void TreeNode::detach(TreeNode *node, TreeNode *parent)
{
    parent->children.removeOne(node);
    node->parent = NULL;
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
                             TreeNode* highlighted,
                             QList<TreeNode*> selectionList)
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

    // Determine if selected
    if (selectionList.contains(this))
        result += "●";

    // Shows highlighting on parent if the highlighted node is an invisible
    // placeholder
    if ((highlighted->isPlaceHolder() && highlighted->parent == this) ||
            highlighted == this)
    //if (highlighted == this) // Alternate to show actual highlighting
        result += " ←"; //◀

    result += "\n";

    // The following lines can be used to actually depict placeholders on the
    // tree. Since placeholders are technically not nodes (they aren't
    // considered when doing operations like BFS, for example), I'm going to
    // leave these lines commented out.
    /*
    if (placeHolderChild)
        result += placeholder->getBoxLine(depth+1,
                                          end,
                                          children.size() == 0,
                                          skips,
                                          highlighted,
                                          selectionList);
    */

    // Now figure out all the children
    QList<QString> childRows;
    for (int i = 0; i < children.size(); ++i)
    {
        TreeNode* child = children.at(i);
        QString childRow = child->getBoxLine(depth + 1,
                                             end,
                                             i == children.size() - 1,
                                             skips,
                                             highlighted,
                                             selectionList);
        childRows.append(childRow);
    }

    // Combine all the child rows here and append them to result
    for (auto s : childRows)
        result += s;

    return result;
}


//////////////
/// Pounce ///
//////////////

/*
 * Turns a decimal int into 2 bits base 4. These two bits correspond to letters
 * on the keyboard. These will be typed in sequence, one after the other.
 *
 * For numbers [0,15]:
 *      * First bit is either A,S,D,F
 *      * Second bit is either J,K,L,;
 *
 * Example: 10 becomes 22 in base 4, this corresponds with the second letter of
 * the first bit (S) followed by the second letter of the second bit (K).
 *
 * This means that you can jump to node 10 by pressing SK in rapid succession.
 *
 * For numbers [16,31], the order is swapped:
 *      * First bit is now J,K,L,;
 *      * Second bit is A,S,D,F
 *
 * Example: 26 is 122 in binary. We look at the last two digits (leading 1 is
 * chopped off) and do the same process but with the right hand going first.
 *
 * The new sequence is now KS, which will bring you to node 26.
 *
 * NOTE: the way I set pounce up as a two key sequence of the home row only
 * means that there is a maximum of 32 different nodes that are possible targets
 * to pounce to. If this is an issue, I can extend it to 50 possible nodes just
 * by adding in the G and H keys. The problem is that they're not in the default
 * positions that the fingers are in, so it adds some minor difficulty to
 * pouncing.
 *
 * I'm keeping it at 32 max nodes since I don't believe that graphs will become
 * too big that they really need additional pounce space. I could be wrong, but
 * we'll have to reexamine this down the line.
 *
 * Params:
 *      target: the unique # identifying this node on its tree
 */
void TreeNode::setPounceID(int target)
{
    // Reset the ID
    pounceID = "";

    // A is the left bit, B is the right bit
    int a, b;

    // Swap worthy
    bool swap = false;

    // Determine if we should swap bits
    if (target > 15)
    {
        target -= 16;
        swap = true;

        // Still too big for this system
        if (target > 15)
        {
            pounceID = "??";
            return;
        }
    }

    a = target / 4;
    b = target % 4;

    if (a == 0)
        pounceID += "A";
    else if (a == 1)
        pounceID += "S";
    else if (a == 2)
        pounceID += "D";
    else if (a == 3)
        pounceID += "F";

    if (b == 0)
        pounceID += "J";
    else if (b == 1)
        pounceID += "K";
    else if (b == 2)
        pounceID += "L";
    else if (b == 3)
        pounceID += ";";

    // Swap the left and right bits on values larger than 15
    if (swap)
    {
        QChar bit0 = pounceID.at(1);
        QChar bit1 = pounceID.at(0);
        pounceID = "";
        pounceID[0] = bit0;
        pounceID[1] = bit1;
    }
}


/*
 * Analogous to getBoxedWidth, see documentation there
 */
int TreeNode::getPounceWidth(int depth)
{
    int myRowLength = (depth * 3) + pounceID.size();

    int childLength = 0;
    for (auto child : children)
        childLength = std::max(child->getPounceWidth(depth+1),
                               childLength);

    return std::max(myRowLength,childLength);
}

/*
 * Analogous to getBoxedLine, see documentation there
 */
QString TreeNode::getPounceLine(int depth, int end, bool bottom,
                                QString skips,
                                TreeNode *highlighted,
                                QList<TreeNode *> selectionList)
{
    QString result = "│ ";

    // Root does less work
    if (isRoot())
    {
        result += pounceID;
        for (int i = 0; i < end - pounceID.size(); ++i)
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
        result += pounceID;

        // Add the remaining space
        int used = (3 * (depth - 1)) + pounceID.size() + 3;
        for (int i = 0; i < (end - used); ++i )
            result += " ";
    }

    // End of the row: no selection / highlight info needed in the pounce box
    result += " │\n";

    // Now figure out all the children
    QList<QString> childRows;
    for (int i = 0; i < children.size(); ++i)
    {
        TreeNode* child = children.at(i);
        QString childRow = child->getPounceLine(depth + 1,
                                             end,
                                             i == children.size() - 1,
                                             skips,
                                             highlighted,
                                             selectionList);
        childRows.append(childRow);
    }

    // Combine all the child rows here and append them to result
    for (auto s : childRows)
        result += s;

    return result;
}
