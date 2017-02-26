#include "ctreestateaddcut.h"

/* Adds child cut */
void CTreeStateAddCut::execute()
{
    node = tree->addChildCut();
    tree->redraw();
}

/* Removes the added cut */
void CTreeStateAddCut::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the cut */
void CTreeStateAddCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddCut::copy()
{
    return new CTreeStateAddCut(tree);
}

