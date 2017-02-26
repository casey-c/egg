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
    tree->selectSpecific(previous);
    tree->redraw();
}

/* Reselects the parent */
void CTreeStateSelectParent::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectParent::copy()
{
    return new CTreeStateSelectParent(tree);
}
