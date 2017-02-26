#include "ctreestateselectachild.h"

/* Selects a child */
void CTreeStateSelectAChild::execute()
{
    tree->selectAChild();
    tree->redraw();
}

/* Selects parent to undo the child selection */
void CTreeStateSelectAChild::undo()
{
    tree->selectParent();
    tree->redraw();
}

/* Reselects a child */
void CTreeStateSelectAChild::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateSelectAChild::copy()
{
    return new CTreeStateSelectAChild(tree);
}
