#include "ctreestateselectparent.h"

/* Selects the parent */
bool CTreeStateSelectParent::execute()
{
    // Remember the previous selection
    previous = tree->getSelected();

    tree->selectParent();

    // No change
    if (tree->getSelected() == previous)
        return false;

    tree->redraw();
    return true;
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
