#ifndef TREE_TREESTATE_H
#define TREE_TREESTATE_H

#include "treenode.h"
#include "Utilities/Command/commandinvoker.h"
#include <QObject>

/*
 * A TreeState stores the root element of an existential graph tree. It also
 * provides an interface to interact with the graph itself (such as selecting a
 * region and handling a modify command), which will handle any substantial
 * changes within the graph.
 *
 * The constructor makes a new root node and selects it.
 *
 * Change selection will update the selected tree node (which will serve as
 * the source of modifications, additions, and removals).
 *
 * Additions will try and add a child of a specific type to the selected
 * region. If the selected region is a statement (which is not allowed to have
 * children of its own), then the addition will add it as a child of the
 * selected region's parent (i.e. make it a new sibling to the selected
 * statement).
 *
 * Removals come in two flavors: saveOrphans will transfer all of the selected
 * region's children to their grandparent; burnTheOrphanage will remove and kill
 * all of the selected region's children if they have any. In either case, the
 * selected region will be removed from the tree. Root elements are not allowed
 * to be removed.
 *
 * Surround with cut simply adds a cut above the selected region in the tree.
 */
class TreeState : public QObject
{
    Q_OBJECT

public:
    TreeState(): root(new TreeNode()), highlighted(root) {}
    //TreeState(TreeState* original):
        //root(new TreeNode(original->copyRoot())), selected(root){}
    ~TreeState();

    /* Highlighted */
    void highlightChild();
    void highlightParent();
    void highlightRoot();

    void highlightRightSibling();
    void highlightLeftSibling();
    void highlightSpecific(TreeNode* node);

    TreeNode* getHighlighted() { return highlighted; }

    /* Selection */
    void selectSpecific(TreeNode* node);
    void selectHighlighted();
    void selectChildrenOfHighlighted();

    void clearSelection();
    void deselectNode(TreeNode* node);
    void deselectHighlighted();

    QList<TreeNode*> getSelectionList() { return selectionList; }
    void setSelectionList(QList<TreeNode*> list) { selectionList = list; }

    /* Add */
    void addChildCut();
    void addChildDoubleCut();
    void addChildStatement(QString s);

    void addOrTemplate();
    void addConditionalTemplate();
    void addBiconditionalTemplate();

    /* Remove */
    void removeAndSaveOrphans(TreeNode* target);
    void removeAndBurnTheOrphanage(TreeNode* target);

    void detachNode(TreeNode* target);
    void detachNodes();

    /* Surround with cut */
    void surroundWithCut();
    void surroundWithDoubleCut();

    /* Copy */
    //static TreeState* copyState(TreeState* currentTree);
    //TreeNode* copyRoot(){ return new TreeNode(this->root); }

    /* Move */
    void move(TreeNode* target, TreeNode* targetParent);

    /* Modification mode */
    TreeState* doubleCutRemoval();
    TreeState* doubleCutAddition();
    void setIterationTarget();
    QList<TreeNode*> getIterationTarget() { return iterationList; }
    TreeState* performIteration();
    TreeState* performDeiteration();

    /* Text output in tree form */
    QString getBoxedString();

    /* Update any views */
    void redraw();


    /* Command helpers */
    QList<TreeNode*> popRecentNodes();
    QList<TreeNode*> popRecentParents();

signals:
    void treeChanged(QString s);

private:
    TreeNode* root;

    /* Multiselect */
    QList<TreeNode*> selectionList; // All nodes selected
    TreeNode* highlighted; // Node underneath the cursor

    QList<TreeNode*> recentUpdatedNodes; // Most recent added nodes (for commands)
    QList<TreeNode*> recentParents; // For detach commands

    QList<TreeNode*> iterationList;
    QString getFormattedString();
};

#endif // TREE_TREESTATE_H
