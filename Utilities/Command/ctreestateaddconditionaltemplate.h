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
    /* Constructor */
    CTreeStateAddConditionalTemplate(TreeState* t) :
        tree(t) { text = "Add conditional"; }

    /* Destructor */
    ~CTreeStateAddConditionalTemplate() {}

    void execute();
    void undo();
    void redo();

    ICommand* copy();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDCONDITIONALTEMPLATE_H
