#ifndef CTREESTATEADDBICONDITIONALTEMPLATE_H
#define CTREESTATEADDBICONDITIONALTEMPLATE_H

#include "Tree/treenode.h"
#include "Tree/treestate.h"

class CTreeStateAddBiconditionalTemplate : public ICommand
{
public:
    CTreeStateAddBiconditionalTemplate(TreeState* t) :
        tree(t) {}

    void execute();
    void undo();
    void redo();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDBICONDITIONALTEMPLATE_H
