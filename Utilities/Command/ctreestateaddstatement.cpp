#include "ctreestateaddstatement.h"

/* Adds child statement */
bool CTreeStateAddStatement::execute()
{
    TreeNode* previous = tree->getSelected();
    node = tree->addChildStatement(statement);

    // Didn't add the statement
    if (previous == node)
        return false;

    tree->redraw();
    return true;
}

/* Removes the added child statement */
void CTreeStateAddStatement::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Copy */
ICommand* CTreeStateAddStatement::copy()
{
    return new CTreeStateAddStatement(tree,statement);
}
