#include "ctreestateselectright.h"

/* Selects right */
bool CTreeStateSelectRight::execute()
{
    TreeNode* previousNode = tree->getSelected();
    tree->selectRightSibling();

    // Didn't change selection
    if (tree->getSelected() == previousNode)
        return false;

    tree->redraw();
    return true;
}

/* Reverts the selection by selecting left */
void CTreeStateSelectRight::undo()
{
    tree->selectLeftSibling();
    tree->redraw();
}

/* Copy */
ICommand* CTreeStateSelectRight::copy()
{
    return new CTreeStateSelectRight(tree);
}
