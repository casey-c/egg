#ifndef CTREESTATESELECTLEFT_H
#define CTREESTATESELECTLEFT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command will call the select left function on a tree state
 */
class CTreeStateSelectLeft : public ICommand
{
public:
    /* Constructor */
    CTreeStateSelectLeft(TreeState* t) :
        tree(t) { text = "Select left"; }

    /* Destructor */
    ~CTreeStateSelectLeft() {}

    void execute();
    void undo();
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
};

#endif // CTREESTATESELECTLEFT_H
