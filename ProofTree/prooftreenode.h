#ifndef PROOFTREENODE_H
#define PROOFTREENODE_H

#include "Tree/treestate.h"
#include <QList>

/*
 * A ProofTreeNode serves as a wrapper class for TreeStates. It provides
 * additional ProofTree specific uses (like storing pointers to proceeding and
 * preceeding steps in the proof tree) without cluttering up the TreeState class
 * any further.
 *
 * Nodes are responsible for creating other nodes, the "root" is the premise
 * graph stored in the ProofTree object.
 *
 * This class uses the named constructor idiom: only premise nodes and goal
 * nodes can be created outside this class. These can be constructed through the
 * supplied factory methods.
 *
 * Other nodes (the modification steps / PT_INNER) are produced through the
 * addNewChildNode() method. This handles all the proper pointer linking and
 * type setting. If used correctly, this should also be a safer way of
 * maintaining the tree structure, as the nodes themselves are responsible for
 * creating new child nodes.
 *
 * Goal nodes aren't actually connected to the tree structure. They are saved
 * as the "solution" of a particular graph target: i.e. where the user wants to
 * end up after completing lots of inferences / modifications to the original
 * premise graph.
 *
 * As such, goal nodes cannot add any children. Child nodes must extend from
 * the premise node or any other inner nodes.
 *
 * This class takes advantage of two sets of constants:
 *      * PT_*: the nodeType gives certain properties
 *              ** PT_PREMISE:  root premise node (no parent, may have kids)
 *              ** PT_GOAL:     goal node (unconnected)
 *              ** PT_INNER:    modification node (has parent, may have kids)
 *
 *      * STEP_*: these are used for PT_INNER nodes as modificationType
 *              ** STEP_DC_ADD: this step adds a double cut
 *              ** STEP_DC_REMOVE: this step removes a double cut
 *              ** STEP_ITERATE: this step iterated off the previous tree
 *              ** STEP_DEITERATE: this step deiterated off the previous tree
 *
 * NOTE: only inner nodes contain the STEP_ type. They are used mostly just to
 * display a text string of what the step actually did, so you can quickly look
 * back through the proof and see what kinds of inferences were needed.
 */
class ProofTreeNode
{
public:
    // Factory constructor makes a PT_PREMISE node
    static ProofTreeNode* makePremiseNode();

    // Factory constructor makes a PT_GOAL node
    static ProofTreeNode* makeGoalNode();

    /* Additions */
    ProofTreeNode* addNewChildNode(TreeState* state, int type);

    /* Boolean identifiers */
    bool isPremise() { return nodeType == constants::PT_PREMISE; }
    bool isGoal() { return nodeType == constants::PT_GOAL; }
    bool isInner() { return nodeType == constants::PT_INNER; }

    bool isLeaf() { return children.isEmpty(); }

    /* Removal */
    // TODO: this clear function is to maintain a simple path, no branching is
    // implemented yet for simplicity
    void clearChildren() { children.clear(); } // TODO: fix mem leak

private:
    // Private constructor creates inner nodes
    ProofTreeNode();

    int nodeType; // constants::PT_*
    int modificationType; // constants::STEP_*

    QList<ProofTreeNode*> children;
};

#endif // PROOFTREENODE_H
