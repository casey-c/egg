#ifndef CTREESTATEADDSTATEMENT_H
#define CTREESTATEADDSTATEMENT_H

#include "icommand.h"
#include "Tree/treestate.h"

/*
 * This command will add a statement to a tree state.
 */
class CTreeStateAddStatement : public ICommand
{
public:
    /* Constructor */
    CTreeStateAddStatement(TreeState* t, QString s) :
        tree(t),
        statement(s) { text = "Add "+statement; }

    /* Destructor */
    ~CTreeStateAddStatement() {}

    void execute();
    void undo();
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
    QString statement;

    TreeNode* node;
};

#endif // CTREESTATEADDSTATEMENT_H
