#ifndef CTREESTATESELECTRIGHT_H
#define CTREESTATESELECTRIGHT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command will call the select right function on a tree state
 */
class CTreeStateSelectRight : public ICommand
{
public:
    /* Constructor */
    CTreeStateSelectRight(TreeState* t) :
        tree(t) { text = "Select right"; }

    /* Destructor */
    ~CTreeStateSelectRight() {}

    bool execute();
    void undo();

    ICommand* copy();

private:
    TreeState* tree;
};

#endif // CTREESTATESELECTRIGHT_H
