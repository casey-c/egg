#ifndef PROOFTREENODE_H
#define PROOFTREENODE_H


/*
 * A ProofTreeNode serves as a wrapper class for TreeStates. It provides
 * additional ProofTree specific uses (like storing pointers to proceeding and
 * preceeding steps in the proof tree) without cluttering up the TreeState class
 * any further.
 */
class ProofTreeNode
{
public:
    ProofTreeNode();
};

#endif // PROOFTREENODE_H
