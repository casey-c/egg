#ifndef CTREESTATESELECTACHILD_H
#define CTREESTATESELECTACHILD_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command will call the select child function on a tree state
 */
class CTreeStateSelectAChild : public ICommand
{
public:
    /* Constructor */
    CTreeStateSelectAChild(TreeState* t) :
        tree(t) {}

    /* Destructor */
    ~CTreeStateSelectAChild();

    void execute();
    void undo();
    void redo();

private:
    TreeState* tree;
};

#endif // CTREESTATESELECTACHILD_H
