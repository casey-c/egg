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
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Re-add the statement after undo */
void CTreeStateAddStatement::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddStatement::copy()
{
    return new CTreeStateAddStatement(tree,statement);
}
