#include "ctreestateaddstatement.h"

/* Adds child statement */
void CTreeStateAddStatement::execute()
{
    node = tree->addChildStatement(statement);
    tree->redraw();
}

/* Removes the added child statement */
void CTreeStateAddStatement::undo()
{
    tree->removeAndBurnTheOrphanage(node);
    tree->redraw();
}

/* Re-add the statement after undo */
void CTreeStateAddStatement::redo()
{
    execute();
}
