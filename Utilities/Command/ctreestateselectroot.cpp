#include "ctreestateselectroot.h"

/* Selects root */
bool CTreeStateSelectRoot::execute()
{
    // Remember the previous selection
    previous = tree->getSelected();

    // Already at root
    if (previous->isRoot())
        return false;

    tree->selectRoot();
    tree->redraw();
    return true;
}

/* Returns to the previous selection */
void CTreeStateSelectRoot::undo()
{
    tree->selectSpecific(previous);
    tree->redraw();
}

/* Reselects root */
void CTreeStateSelectRoot::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectRoot::copy()
{
    return new CTreeStateSelectRoot(tree);
}
