#include "ctreestateselectleft.h"

/* Selects left */
bool CTreeStateSelectLeft::execute()
{
    TreeNode* previous = tree->getSelected();
    tree->selectLeftSibling();

    // No change
    if (previous == tree->getSelected())
        return false;

    tree->redraw();
    return true;
}

/* Reverts the selection by selecting right */
void CTreeStateSelectLeft::undo()
{
    tree->selectRightSibling();
    tree->redraw();
}

/* Selects left again */
void CTreeStateSelectLeft::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectLeft::copy()
{
    return new CTreeStateSelectLeft(tree);
}
