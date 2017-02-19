#ifndef TREE_TREENODE_H
#define TREE_TREENODE_H

#include<QList>
#include <QString>

#include "Utilities/constants.h"

/*
 * A GraphElement is the basic node of an existential graph tree. These elements
 * can be one of four types: root, cut, statement, or placeholder.
 *
 * Out of these four, only the placeholder is allowed to change its type after
 * creation. The placeholder is special: calling one of the addChild functions
 * will simply replace the current GraphElement instead of creating a new node
 * further down the tree.
 *
 * Root and cut elements are allowed to have children of their own. These
 * are created and returned as the return value of the addChild set of
 * functions.
 *
 * Root elements do not have a parent, and there is only one root per graph
 * tree. Every other node in the tree extends from some branching off of the
 * root.
 *
 * GraphElements are only allowed to have zero or one child placeholders at
 * any time. Placeholders are automatically leaves of the tree and may not
 * have placeholders of their own.
 */

class TreeNode
{
public:
    /* Public factory constructor */
    static TreeNode Root();

    /* TODO: deallocate dynamic memory on destruction of a tree */
    ~TreeNode() {}

    TreeNode* addChildCut();
    TreeNode* addChildStatement(QString s);
    TreeNode* addChildPlaceholder();

    /* TODO: fix rep. exposure (make private? friend graphstate?) */
    QList<TreeNode*> getChildren() { return children; }
    TreeNode* getParent() { return parent; }

    /* Boolean identifiers */
    bool isRoot() { return type == constants::ELEMENT_ROOT; }
    bool isCut() { return type == constants::ELEMENT_CUT; }
    bool isStatement(){ return type == constants::ELEMENT_STATEMENT; }
    bool isPlaceHolder() { return type == constants::ELEMENT_PLACEHOLDER; }

private:
    /* Private default constructor */
    TreeNode(const int t, TreeNode* p, QString n)
            : type(t)
            , parent(p)
            , name(n)
            , placeHolderChild() {}

    int type;
    TreeNode* parent;
    QList<TreeNode*> children;
    QString name;

    TreeNode* placeholder;
    bool placeHolderChild;
};

#endif // TREE_TREENODE_H
