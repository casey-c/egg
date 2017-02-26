#ifndef CTREESTATEADDCONDITIONALTEMPLATE_H
#define CTREESTATEADDCONDITIONALTEMPLATE_H

#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * This command adds a conditional template to a TreeState
 */
class CTreeStateAddConditionalTemplate : public ICommand
{
public:
    CTreeStateAddConditionalTemplate(TreeState* t) :
        tree(t) {}
    ~CTreeStateAddConditionalTemplate();

    void execute();
    void undo();
    void redo();

private:
    TreeState* tree;
    TreeNode* node;

};

#endif // CTREESTATEADDCONDITIONALTEMPLATE_H
