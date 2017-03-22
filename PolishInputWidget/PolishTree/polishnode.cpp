#include "polishnode.h"

///////////////
/// Destroy ///
///////////////

/*
 * Recursively clean up memory
 */
PolishNode::~PolishNode()
{
    for (PolishNode* child : children)
        delete child;
}

/////////////////////////
/// Create and update ///
/////////////////////////

/*
 * Constructs a new child node without any details (essentially making a
 * placeholder)
 */
PolishNode* PolishNode::makeNewChild()
{
    // Can't add a child to a node with arity 0
    if (!op)
        return nullptr;

    // New nodes have no details yet
    PolishNode* newNode = new PolishNode(this, "___", false, false);
    children.prepend(newNode);
    return newNode;
}

/*
 * Sets the details for this node.
 *
 * Params:
 *      t: the text to be displayed by this node
 *              (could be an operator or a letter)
 *      o: whether or not this node is an operator (i.e. should have children)
 */
void PolishNode::setDetails(QString t, bool o)
{
    text = t;
    op = wrapped = o;
}

/////////////
/// Print ///
/////////////

/*
 * Prints the polish notation subtree of this node into something readable by
 * humans. Placeholder nodes are represented by " ___ " and get filled in from
 * left to right.
 *
 * This function is called on the root node every time the tree is changed. This
 * ensures that the UI properly updates to display a readable text box that
 * makes sense to the user (as opposed to directly printing the polish input,
 * which is confusing as hell).
 */
QString PolishNode::toPlaintext()
{
    QString result = "";

    // Leading parenthesis
    if (wrapped && (children.size() > 1))
        result += "(";

    // Non operators just include their text
    if (!op)
        result += text;

    // Operators
    else
    {
        // Arity 1
        if (children.size() == 1)
            result += text + children.at(0)->toPlaintext();

        // Arity >1
        else
        {
            for (int i = 0; i < children.size(); ++i)
            {
                result += children.at(i)->toPlaintext();

                // Adds this node's text as the operator
                if (i != children.size() - 1)
                    result += " " + text + " ";
            }
        }
    }

    // Closing parenthesis
    if (wrapped && (children.size() > 1))
        result += ")";

    return result;
}
