#include "ctreestateselectachild.h"

/* Selects a child */
bool CTreeStateSelectAChild::execute()
{
    TreeNode* previous = tree->getSelected();
    tree->selectAChild();

    // No change
    if (previous == tree->getSelected())
        return false;

    tree->redraw();
    return true;
}

/* Selects parent to undo the child selection */
void CTreeStateSelectAChild::undo()
{
    tree->selectParent();
    tree->redraw();
}

/* Reselects a child */
void CTreeStateSelectAChild::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectAChild::copy()
{
    return new CTreeStateSelectAChild(tree);
}
