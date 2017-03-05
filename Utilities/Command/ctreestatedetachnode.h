#ifndef CTREESTATEDETACHNODE_H
#define CTREESTATEDETACHNODE_H

#include "itreestateremove.h"

/*
 * This command detaches (removes but doesn't delete) nodes of a TreeState
 */
class CTreeStateDetachNode : public ITreeStateRemove
{
public:
    /* Constructor */
    CTreeStateDetachNode(TreeState* t) {
        text = "Remove node";
        tree = t;
    }

    /*
     * Destructor:
     * TODO: delete the detached node, so we no longer have a memory leak if
     * this command is deleted
     */
    ~CTreeStateDetachNode() {}

    /* Override the remove() function of the ITreeStateRemove interface */
    void remove() { tree->detachNodes(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateDetachNode(tree); }
};

#endif // CTREESTATEDETACHNODE_H
