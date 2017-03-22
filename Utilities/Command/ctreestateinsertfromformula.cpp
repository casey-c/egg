#include "ctreestateinsertfromformula.h"

bool CTreeStateInsertFromFormula::execute()
{
    parent = tree->getHighlighted();
    movedNodes = src->getChildren();

    // Nothing to move
    if (movedNodes.isEmpty())
        return false;

    // Select the parent, if statement (will add as siblings)
    if (parent->isStatement())
        parent = parent->getParent();

    // Similarly for placeholders (the placeholder will be filled anyway)
    else if (parent->isPlaceHolder())
        parent = parent->getParent();

    // Move them into the actual tree
    for (TreeNode* node : movedNodes)
        tree->move(node, parent);

    // Redraw the tree
    tree->redraw();
    return true;
}

void CTreeStateInsertFromFormula::undo()
{
    // Take all the moved nodes and give them back to the original (src) root
    for (TreeNode* node : movedNodes)
        TreeNode::move(node, src);

    // Redraw the tree
    tree->redraw();
}
