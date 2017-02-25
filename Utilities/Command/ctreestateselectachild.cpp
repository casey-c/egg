#include "ctreestateselectachild.h"

/* Selects a child */
void CTreeStateSelectAChild::execute()
{
    // Remember the previous selection
    previous = tree->getSelected();

    tree->selectAChild();
    tree->redraw();
}

/* Reverts to previous selection */
void CTreeStateSelectAChild::undo()
{
    // TODO: implementation
}

/* Reselects a child */
void CTreeStateSelectAChild::redo()
{
    execute();
}
