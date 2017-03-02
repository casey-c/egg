#ifndef CTREESTATEADDORTEMPLATE_H
#define CTREESTATEADDORTEMPLATE_H

#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * This command adds an or template to a Tree State
 */
class CTreeStateAddOrTemplate : public ICommand
{
public:
    /* Constructor */
    CTreeStateAddOrTemplate(TreeState* t) :
        tree(t) { text = "Add OR template"; }

    /* Destructor */
    ~CTreeStateAddOrTemplate() {}

    bool execute();
    void undo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDORTEMPLATE_H
