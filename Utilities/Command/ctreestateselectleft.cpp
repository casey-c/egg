#include "ctreestateselectleft.h"

/* Selects left */
void CTreeStateSelectLeft::execute()
{
    tree->selectLeftSibling();
    tree->redraw();
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
