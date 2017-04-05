#include "ctreestateinsertfromformula.h"

/*
 * Destructor
 */
CTreeStateInsertFromFormula::~CTreeStateInsertFromFormula()
{
    // TODO: implementation
}


/*
 * Puts the new nodes into the current tree at its highlighted node.
 *
 * If the highlighted node is a statement or placeholder, the nodes are moved to
 * the highlighted node's parent instead.
 *
 * Returns:
 *      true: if there are nodes to move
 *      false: otherwise
 */
bool CTreeStateInsertFromFormula::execute()
{
    qDebug() << "executing command";
    parent = tree->getHighlighted();
    movedNodes = src->getChildren();

    // Nothing to move
    if (movedNodes.isEmpty())
        return false;

    qDebug() << "made it 1";
    // Select the parent, if statement (will add as siblings)
    if (parent->isStatement())
        parent = parent->getParent();

    qDebug() << "made it 2";
    // Similarly for placeholders (the placeholder will be filled anyway)
    if (parent->isPlaceHolder())
        parent = parent->getParent();

    qDebug() << "made it 3";
    // Move them into the actual tree
    for (TreeNode* node : movedNodes)
        tree->move(node, parent);
    qDebug() << "made it 4";

    // Redraw the tree
    tree->redraw();
    return true;
}

/*
 * Pulls the moved nodes back out of the tree and back into children of the
 * root node created in the InsertFromFormula window. This root (src) node is
 * invisible to the user.
 */
void CTreeStateInsertFromFormula::undo()
{
    // Take all the moved nodes and give them back to the original (src) root
    for (TreeNode* node : movedNodes)
        TreeNode::move(node, src);

    // Redraw the tree
    tree->redraw();
}
