#include "Tree/treenode.h"
#include "Tree/treestate.h"
#include <algorithm>

#include <QDebug>

/* Debug id */
int TreeNode::globalID = 0;

//////////////////////////////////
/// Constructors / Destructors ///
//////////////////////////////////

/* Copy constructor */
TreeNode::TreeNode(TreeNode *original):
    type(original->getType()),
    name(original->getName()),
    numPlaceholderChildren(original->numPlaceholderChildren),
    //placeHolderChild(false),
    myID(globalID++)
{
    //if (original->hasPlaceHolder())
        //addChildPlaceholder();

    for (TreeNode* child : original->getChildren())
    {
        TreeNode* node = new TreeNode(child);
        node->parent = this;
        children.append(node);
    }
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
    //if (placeHolderChild)
        //delete placeholder;
}

//////////////////////////
///      Equality      ///
//////////////////////////

/*
 *  Compares this node with another input node and determines if they are EQUAL.
 *
 *  EQUAL means that two of the nodes have the same structure of tree.
 *  The order of their children can be different but the number, type, and name
 *  (if it is a statement) should be same to be considered EQUAL.
 *
 *  Input : node - a node to be compared with this node
 *
 *  effects : compares two trees from this and input node as a root.
 *
 *  returns : true or false if EQUAL or not
 */
bool TreeNode::equals(TreeNode* node1, TreeNode* node2)
{
    QList<int> cutCount1, cutCount2, depths1, depths2;
    QList< QList<QString> > list1, list2;

    //Get lists of leaves
    TreeNode::getLeaves(node1, list1, cutCount1, depths1);
    TreeNode::getLeaves(node2, list2, cutCount2, depths2);

    //Comparison begins
    if(list1.size() != list2.size() || cutCount1.size() != cutCount2.size()
            || depths1.size() != depths2.size())
        return false;

    for (int i = 0; i < depths1.size(); i++)
    {
        //Compare each depth for each index
        if(depths1[i] != depths2[i])
            return false;

        //Compare the number of cuts for each depth
        if(cutCount1[i] != cutCount2[i])
            return false;

        //Compare size of each second index of lists
        if(list1[i].size() != list2[i].size())
            return false;

        //Sorting each second list of statement(run time of O(n*logn))
        std::sort(list1[i].begin(), list1[i].end());
        std::sort(list2[i].begin(), list2[i].end());

        //Compare the sorted strings of statement in each depths
        for (int j = 0; j < list1[i].size(); i++)
        {
            if(list1[i][j] != list2[i][j])
                return false;
        }
    }

    //If it hasn't been returned false then two leaves lists are same
    //so return true
    return true;
}

/*
 *  Helper function for isEqualWith()
 *
 *  From the input node, we will BFS search and find the leaves(cut/statements)
 *  and update the input QLists based off the BFS search
 *
 *  Statement list is still unsorted after this function
 */
void TreeNode::getLeaves(TreeNode* root, QList< QList<QString> > &list,
                         QList<int> &cutCount, QList<int> &depths)
{
    QQueue<TreeNode*> queue;
    int currentDepth = 0, currentIndex = -1;

    //Special case for root not having any children
    //Will just return without any effect
    if(root->getChildren().isEmpty())
        return;

    //BFS search starts here
    queue.enqueue(root);

    while (!queue.isEmpty())
    {
        TreeNode* node = queue.dequeue();

        //whenever we find a leaf (has no child)
        if(node->getChildren().isEmpty())
        {
            //If this leaf is a placeholder then just skip it
            if(node->getType() == constants::ELEMENT_PLACEHOLDER)
                continue;

            if(node->getDepth() == currentDepth)
            {
                //If same depth was already found,
                //For cut, increment cutCount of current index
                //For statement, append this node name to current index's list
                if(node->getType() == constants::ELEMENT_CUT)
                    cutCount[currentIndex]++;
                else if(node->getType() == constants::ELEMENT_STATEMENT)
                    list[currentIndex].append(node->getName());
            }
            else
            {
                //Create a list for new depth and if the leaf is a statement
                //then add it to the new list
                QList<QString> newIndex;
                if(node->getType() == constants::ELEMENT_STATEMENT)
                    newIndex.append(node->getName());
                list.append(newIndex);

                //Append the cutCount and if the leaf is a cut then
                //increment by 1
                cutCount.append(0);
                if(node->getType() == constants::ELEMENT_CUT)
                    cutCount.last()++;

                //Append depth with new node's depth to parallelize the lists
                depths.append(node->getDepth());

                currentDepth = node->getDepth();
                currentIndex++;
            }
        }

        for (TreeNode* child : node->getChildren())
            queue.enqueue(child);
    }
}

/*
 *  Updates the depth of this and all of the children nodes by using BFS
 */
void TreeNode::updateDepth()
{
    QQueue<TreeNode*> queue;

    queue.enqueue(this);

    while (!queue.isEmpty())
    {
        TreeNode* node = queue.dequeue();

        // update depth here
        node->depth = parent->getDepth() + 1;

        for (TreeNode* child : node->getChildren())
            queue.enqueue(child);
    }
}

/////////////////
/// Additions ///
/////////////////

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

        // First, remove us from the front of the children list as a placeholder
        parent->children.removeOne(this);

        // Update our parent's placeholder status by looking for another
        //parent->updatePlaceholderStatus();
        parent->numPlaceholderChildren--;

        // Put us at the back of our parent's children list as a real cut
        parent->children.append(this);
        return this;
    }

    // See if we have a placeholder child to replace
    else if (numPlaceholderChildren > 0)
    {
        // Get that element
        TreeNode* placeholder = children.first();

        // Use the above logic for placeholders
        return placeholder->addChildCut();
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

        // Remove us from our parent's children
        parent->children.removeOne(this);

        // Update our parent's placeholder check
        //parent->updatePlaceholderStatus();
        parent->numPlaceholderChildren--;

        // Add us back in as a real statement in the proper place
        parent->addAfterPlaceholders(this);
        return this;
    }

    // See if we have a placeholder child to replace
    else if (numPlaceholderChildren > 0)
    {
        // Get that element
        TreeNode* placeholder = children.first();

        // Use the logic defined above for placeholders
        return placeholder->addChildStatement(s);
    }

    // Otherwise, make a new statement element
    else
    {
        TreeNode* newStatement = new TreeNode(constants::ELEMENT_STATEMENT,
                                              this,
                                              s);

        // And add it to our children in the right place
        addAfterPlaceholders(newStatement);
        return newStatement;
    }
}

/*
 * Adds a child placeholder to this node. If this node is a statement, it cannot
 * have a placeholder, so it returns NULL. If this statement is already a
 * placeholder, it returns itself. If this statement already has a child
 * placeholder, that placeholder is returned (limit 1 placeholder per node).
 * Otherwise, we create a new placeholder node, assign it as a child to this one
 * and then return that created node.
 */
TreeNode* TreeNode::addChildPlaceholder()
{
    // Check if this is allowed to have children
    if (isStatement())
        return NULL;

    // Check if this is already a placeholder
    if (isPlaceHolder())
        return this;

    // Okay to add new placeholder as a child
    TreeNode* newPlaceholder = new TreeNode(constants::ELEMENT_PLACEHOLDER,
                                            this,
                                            NULL);
    children.prepend(newPlaceholder);
    numPlaceholderChildren++;

    return newPlaceholder;
}

////////////
/// Move ///
////////////

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
    target->updateDepth();
}

/*
 * DEBUG:
 * This function checks that if parameter node is in upper family line of this
 * node. (parent, grandparent, grand-grandparent..... and goes on)
 *
 * Returns:
 *      true : if parameter is a part of the upper family line
 *      false : if not
 *
 * Params:
 *      node : node that we are checking if it is in the line
 */
//bool TreeNode::isInUpperFamily(TreeNode* node)
//{
    //TreeNode* compareNode = parent;
    //while (compareNode->getType() != constants::ELEMENT_ROOT)
    //{
        //if(compareNode->getID() == node->getID())
            //return true;
//
        //compareNode = compareNode->getParent();
    //}
    //return false;
//}

//////////////
/// Detach ///
//////////////

/*
 * Detaches a node by disconnecting the parent from the child
 */
void TreeNode::detach(TreeNode *node, TreeNode *parent)
{
    parent->children.removeOne(node);
    node->parent = NULL;
}

///////////////////
/// Text Output ///
///////////////////

/*
 * Changes the type int into a readable QString, for printing on the tree
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
 *              bottom piece, respectively.
 *      highlighted: a pointer to the TreeState's highlighted node; this
 *              determines when to apply the ← symbol to a row
 *      selectionList: a list of the TreeState's selected nodes; this determines
 *              when to apply the ● symbol to a row
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
    //if ((highlighted->isPlaceHolder() && highlighted->parent == this) ||
            //highlighted == this)
    if (highlighted == this) // Alternate to show actual highlighting
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



///////////////////
/// Placeholder ///
///////////////////

/*
 * Checks the first node in children to see if it is a placeholder
 */
//void TreeNode::updatePlaceholderStatus()
//{
    // No children
    //if (children.isEmpty())
        //placeHolderChild = false;
    //else // Check if the first child is a placeholder
        //placeHolderChild = children.first()->isPlaceHolder();
//}

void TreeNode::addAfterPlaceholders(TreeNode *node)
{
    // Find the position of the first non-placeholder element
   // int i = 0;
   // for (; i < children.size(); ++i)
   //     if (!children.at(i)->isPlaceHolder())
   //         break;

    // Insert it there
    children.insert(numPlaceholderChildren, node);
}

///////////////////////
/// Standardization ///
///////////////////////

/*
 * Helper to sort a list of pairs by the first element
 */
bool comparePairs( const QPair<QString, TreeNode*>& first,
                   const QPair<QString, TreeNode*>& second)
{
    return first.first < second.first;
}

/*
 * The intermediate add function. This is called after the node is already
 * created, but before it is added to the children list of this current node.
 *
 * This function will be called during all of the add() family of functions and
 * will attempt to sort all of this nodes' children, and any node above this one
 * that depend on this sorting (i.e. my parent, grandparent, etc. all the way
 * up). This functionality is contained within sortChildren().
 *
 * The sorting process will also construct an output string as a biproduct; this
 * process is produced in updateStringRep().
 *
 * Params
 *      node: the new child I want as my own
 */
bool TreeNode::addAndStandardize(TreeNode *node)
{
    // Null check
    if (node == nullptr)
    {
        qDebug() << "ERROR: nullptr passed to addAndStandardize.";
        return false;
    }

    // Placeholder check
    if (node->isPlaceHolder())
    {
        // Placeholders aren't sorted, and don't affect anyone else
        children.prepend(node);
        return true;
    }

    // Store my old string rep
    QString oldRep = stringRep;

    // Add the new node and call the sorting method
    children.append(node);
    sortChildren();

    // Recalculate our string rep
    updateStringRep();

    // If it's changed, notify our parent to update as well
    if (stringRep != oldRep)
    {
        if (parent != nullptr)
        {
            qDebug() << "Update parent too";
        }
        else
        {
            qDebug() << "NULL parent, so we're done";
        }

    }

    return true;
}

/*
 * When called, it is assumed that all the required children of this node are
 * already well-sorted and have proper string reps already set. This function
 * will then look at the string reps of its kids and sort them alphabetically.
 */
void TreeNode::sortChildren()
{
    // Find out the new node's sibling strings for sorting purposes
    QList< QPair<QString, TreeNode*> > cutPairs;
    QList< QPair<QString, TreeNode*> > statementPairs;

    QList< TreeNode* > placeholderList;
    QList< TreeNode* > cutList;
    QList< TreeNode* > statementList;

    // Separate children into lists segregated by type
    for ( TreeNode* child : children)
    {
        if (child->isPlaceHolder())
        {
            placeholderList.append(child);
            continue;
        }

        QPair< QString, TreeNode*> pair;
        pair.first = child->stringRep;
        pair.second = child;

        if (child->isCut())
        {
            cutPairs.append(pair);
            //cutList.append(child);
        }
        else if (child->isStatement())
        {
            statementPairs.append(pair);
            //statementList.append(child);
        }
    }

    // The new node to be added
    //QPair<QString, TreeNode*> newPair;
    //newPair.first = node->stringRep;
    //newPair.second = node;

    // Now perform the actual sorting
    //if (node->isCut())
    //{
        // Add the new cut to the proper group
        //cutPairs.append(newPair);

        // Sort that group by string rep
        std::sort(cutPairs.begin(), cutPairs.end(), comparePairs);

        // Acquire the updated ordering
        //QList<TreeNode*> sortedCuts;
        for (QPair<QString, TreeNode*> pair : cutPairs)
            cutList.append(pair.second);

        // Replace the old cut list
        //cutList = sortedCuts;
    //}
    //else if (node->isStatement())
    //{
        // Add the new statement to the proper group
        //statementPairs.append(newPair);

        // Sort that group by string rep
        std::sort(statementPairs.begin(), statementPairs.end(), comparePairs);

        // Acquire the updated ordering
        //QList<TreeNode*> sortedStatements;
        for (QPair< QString, TreeNode* > pair : statementPairs)
            statementList.append(pair.second);

        // Replace the old statement list
        //originalStatements = sortedStatements;
    //}

    // Recombine everything
        children.clear();
        //QList< TreeNode* > combined;
        for (TreeNode* n : placeholderList)
            children.append(n);
        for (TreeNode* n : cutList)
            children.append(n);
        for (TreeNode* n : statementList)
            children.append(n);

    // Update our children
    //children = combined;
}

/*
 * A helper function for constructing the updated string rep (not sure if used)
 */
void TreeNode::updateStringRep()
{
    // Use my children's string reps to construct my own
    QString result = "";

    if (isStatement())
    {
        stringRep = name;
        return;
    }
    else if (isCut() || isRoot())
    {
        result += children.size() - numPlaceholderChildren;
    }

    for (TreeNode* child : children)
    {
        if (!child->isPlaceHolder())
            result += child->stringRep;
    }

}
