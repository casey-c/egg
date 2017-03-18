#include "prooftree.h"

////////////////////////////////
/// Constructors/ Destructor ///
////////////////////////////////

ProofTree::ProofTree()
{

}

ProofTree::~ProofTree()
{
    // TODO: implementation
}

/////////////////
/// Additions ///
/////////////////

/*
 * Adds a step as a child to the highlighted ProofNode.
 *
 * Any newly created TreeState (from modification commands) will be linked into
 * the proof tree with this function.
 *
 * Params:
 *      state: the newly created TreeState to be added as a child of the
 *             highlighted node
 *      type:  the type of modification occured on the tree (e.g. STEP_DC_ADD)
 *
 * NOTE: the param should be a modification of the highlighted ProofTreeNode:
 * making a modification to that tree (out of the 4 valid inference and
 * equivalence rules) will produce a command that ends up calling this function
 * with the copied / updated TreeState.
 */
void ProofTree::addStep(TreeState *state, int type)
{
    // NULL check
    if (highlighted == NULL)
        return;

    // Non leaf node means we're up the tree
    if (!highlighted->isLeaf())
    {
        qDebug() << "Other children exist, so clearing them!";

        // Delete the other siblings for simplicity
        highlighted->clearChildren();
    }

    // Add the new tree state as a child to the highlighted node
    highlighted->addNewChildNode(state, type);
}
