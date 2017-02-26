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
    tree->removeAndBurnTheOrphanage(node);
    tree->redraw();
}

/* Readds the cut */
void CTreeStateAddCut::redo()
{
    execute();
}
