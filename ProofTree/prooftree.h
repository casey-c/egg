#ifndef PROOFTREE_H
#define PROOFTREE_H

#include "Tree/treestate.h"
#include "prooftreenode.h"

/*
 * A ProofTree object contains all the potential TreeStates that the graph
 * can hold. These include:
 *      * Premise tree state
 *      * Goal tree state
 *      * Intermediate tree states
 *
 * All of the TreeState objects above are stored in ProofTreeNodes, a type of
 * wrapper class to aid the construction of the tree. These nodes simply
 * encapsulate a TreeState and supply additional helper functions for this
 * class to use and manipulate.
 *
 * The storage of the intermediate tree states are the key to this object's
 * existence. It can store a branching tree structure where the root node is
 * the premise graph, and each downward node of the tree is the resulting
 * TreeState of a certain valid operation.
 *
 * This stores a visible history of valid moves, allowing one to walk through
 * the proof and see the steps they took to get there.
 *
 * While not required, it is encouraged (and is the main point of this app) to
 * have the final leaf node on any branch of the HistoryTree to be equivalent
 * to the goal graph.
 *
 * Upon completing a valid equivalence/inference operation upon a TreeState,
 * the ProofTree will add that step to the end of the branch. After doing so,
 * this object will try and verify its equivalence to the goal graph.
 *
 * If the most recent step is the same as the goal graph, the user will have
 * constructed a verifiable proof that their argument is valid.
 *
 * If the goal graph is a single empty cut (i.e. a contradiction), the user can
 * show an inconsistent set of premises.
 *
 * NOTE: at this point in time, the ProofTree is NOT taking advantage of its
 * tree structure. We are not allowing branching / multiple pathways to the
 * goal. This is for simplicity and time constraints: we are making it like a
 * tree though in the hopes that these features are added later. Additionally,
 * without this branching mechanism, any newly added TreeStates that occur when
 * the highlighted ProofTreeNode is no longer a leaf node (i.e. adding
 * something to a previous step that already has children), the rest of the tree
 * is permanently wiped. This, again, is for simplicity. Our "tree" is nothing
 * more than a single acyclic path (rooted at the premise graph and hopefully
 * ending at the goal).
 */
class ProofTree
{
public:
    /* Constructor */
    ProofTree();

    /* Destructor */
    ~ProofTree();

    /* Additions */
    void addStep(TreeState* state, int type);


    /* Highlight */
    void highlightPrevious();
    void highlightNext();
    void highlightPremise();
    void highlightGoal();

private:
    ProofTreeNode* premise;
    ProofTreeNode* goal;

    ProofTreeNode* highlighted;
};

#endif // PROOFTREE_H
