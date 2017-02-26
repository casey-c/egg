#include "ctreestateselectroot.h"

/* Selects root */
void CTreeStateSelectRoot::execute()
{
    // Remember the previous selection
    previous = tree->getSelected();

    tree->selectRoot();
    tree->redraw();
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
