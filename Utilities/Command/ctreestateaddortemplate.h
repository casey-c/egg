#ifndef CTREESTATEADDORTEMPLATE_H
#define CTREESTATEADDORTEMPLATE_H

#include "Tree/treenode.h"
#include "Tree/treestate.h"

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
