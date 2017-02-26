#include "ctreestateadddoublecut.h"

/* Adds double cut */
void CTreeStateAddDoubleCut::execute()
{
    node = tree->addChildCut();
    tree->addChildCut();

    tree->redraw();
}

/* Removes the added double cut */
void CTreeStateAddDoubleCut::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the double cut */
void CTreeStateAddDoubleCut::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddDoubleCut::copy()
{
    return new CTreeStateAddDoubleCut(tree);
}
