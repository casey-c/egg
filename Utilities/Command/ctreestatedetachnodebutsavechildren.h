#ifndef CTREESTATEDETACHNODEBUTSAVECHILDREN_H
#define CTREESTATEDETACHNODEBUTSAVECHILDREN_H

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"
#include <QList>

/*
 * This command detaches nodes from a TreeState, but moves all the children
 * to the original node's parent.
 */
class CTreeStateDetachNodeButSaveChildren : public ICommand
{
public:
    /* Constructor */
    CTreeStateDetachNodeButSaveChildren(TreeState* t) : tree(t) {
        text = "Remove node; save children"; // TODO: better undo text here
    }

    /*
     * Destructor:
     * TODO: delete the detached node, so we no longer have a memory leak if
     * this command is deleted
     */
    ~CTreeStateDetachNodeButSaveChildren() {}

    bool execute();
    void undo();

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateDetachNodeButSaveChildren(tree); }

private:
    TreeState* tree;
    TreeNode* prevHighlighted;

    QList<TreeNode*> prevSelected;
    QList<TreeNode*> detachedNodes;
    QList<TreeNode*> oldParents;
    QList< QList<TreeNode*> > relocatedChildren;

};

#endif // CTREESTATEDETACHNODEBUTSAVECHILDREN_H
