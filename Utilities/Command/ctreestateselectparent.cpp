#include "ctreestateselectparent.h"

/* Selects the parent */
void CTreeStateSelectParent::execute()
{
    // Remember the previous selection
    previous = tree->getSelected();

    tree->selectParent();
    tree->redraw();
}

/* Reverts to the previous selection */
void CTreeStateSelectParent::undo()
{
    // TODO: implementation
}

/* Reselects the parent */
void CTreeStateSelectParent::redo()
{
    execute();
}
