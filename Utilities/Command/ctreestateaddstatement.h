#ifndef CTREESTATEADDSTATEMENT_H
#define CTREESTATEADDSTATEMENT_H

#include "icommand.h"
#include "Tree/treestate.h"

/*
 * CTreeStateAddStatement command will add a statement to a TreeState
 */
class CTreeStateAddStatement : public ICommand
{
public:
    CTreeStateAddStatement(TreeState* prev, QString s);
    void execute();
    void undo();
    void redo();

private:
    TreeState* previous;
    TreeState* desired;

    TreeState* current;
};

#endif // CTREESTATEADDSTATEMENT_H
