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
    CTreeStateAddOrTemplate(TreeState* t) :
       tree(t) {}
    ~CTreeStateAddOrTemplate();

    void execute();
    void undo();
    void redo();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDORTEMPLATE_H
