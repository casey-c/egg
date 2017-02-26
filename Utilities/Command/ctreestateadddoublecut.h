#ifndef CTREESTATEADDDOUBLECUT_H
#define CTREESTATEADDDOUBLECUT_H

#include "Tree/treestate.h"
#include "Tree/treenode.h"

/*
 * This command adds a double cut to a TreeState
 */
class CTreeStateAddDoubleCut : public ICommand
{
public:
    CTreeStateAddDoubleCut(TreeState* t) :
        tree(t) {}
    ~CTreeStateAddDoubleCut();

    void execute();
    void undo();
    void redo();

private:
    TreeState* tree;
    TreeNode* node;
};

#endif // CTREESTATEADDDOUBLECUT_H
