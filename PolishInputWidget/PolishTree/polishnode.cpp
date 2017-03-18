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
 * Constructs a new child node without any details (i.e. a placeholder)
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
 * Sets the details for this node
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
 * To plaintext string
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
