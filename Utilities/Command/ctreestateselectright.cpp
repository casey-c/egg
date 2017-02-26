#include "ctreestateselectright.h"

/* Selects right */
void CTreeStateSelectRight::execute()
{
    tree->selectRightSibling();
    tree->redraw();
}

/* Reverts the selection by selecting left */
void CTreeStateSelectRight::undo()
{
    tree->selectLeftSibling();
    tree->redraw();
}

/* Reselects right */
void CTreeStateSelectRight::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectRight::copy()
{
    return new CTreeStateSelectRight(tree);
}
