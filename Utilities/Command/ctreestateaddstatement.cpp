#include "ctreestateaddstatement.h"

/* Constructor */
CTreeStateAddStatement::CTreeStateAddStatement(TreeState* prev, QString s)
{
    previous = prev;

    desired = TreeState(prev);
    desired->addChildStatement(s);
}

/* Update the state to the one with the add */
void CTreeStateAddStatement::execute()
{
    current = desired;
}

/* Revert to the state before the add */
void CTreeStateAddStatement::undo()
{
    current = previous;
}

/* Re-add the statement after undo */
void CTreeStateAddStatement::redo()
{
    current = desired;
}
