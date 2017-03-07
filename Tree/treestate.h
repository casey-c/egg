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
 * The constructor makes a new root node and highlights it.
 *
 * There are a couple of concepts to understand before working with a TreeState:
 *
 * Highlight vs. Selection:
 *      highlight: a single pointer indicating what the mouse is on / where the
 *                 keyboard is, where the focus should be
 *      selection: a collection of sibling nodes that act as an extension of the
 *                 highlighted node; this enables operations to act on multiple
 *                 nodes at once, instead of just the single highlighted node
 *
 *    * Both highlight and selection act similar; if nothing is selected, the
 *      operations will all act on the highlighted node instead. Selections just
 *      allow for multi-selection to repeat operations on many nodes. They are
 *      also critical to the design of iterations and deiterations, which act
 *      upon subgraphs as points of copy and erasure.
 *
 * Basic operations (EDIT mode):
 *      add:       builds the tree by adding nodes beneath the highlighted node
 *                 or the selected nodes
 *      highlight: moves the highlighted node, effectively handling a mouse move
 *                 or a keyboard movement key press
 *      select:    changes the selection list; makes sure we don't violate any
 *                 of the restrictions imposed upon selection
 *      remove:    permanently deleting a node from the tree - this is
 *                 unrecoverable, and is used after undoing an add call
 *      detach:    similar to remove, however the node isn't actually deleted,
 *                 a pointer to the detached node is stored in a command
 *                 somewhere. this is what is called through the detach commands
 *                 (N key, M key) so that the processes are easily undoable and
 *                 nodes don't have to be deleted and remade often; just detach
 *                 and move back into place
 *      move:      essentially changes the parent of a treenode, making it move
 *                 on the tree structure. there's some logic to ensure the tree
 *                 integrity isn't violated (so can't move a root down the tree
 *                 or anything silly like that)
 *      surround:  for surrounding a selection with a cut or double cut
 *
 * Modification operations (MODIFICATION mode):
 *      DC:        surround anything with a double cut, remove any double cut
 *                 that surrounds a node (if the double cut is contiguous)
 *      Insertion: enter EDIT mode on any odd level subgraph
 *      Erasure:   detach anything on an even level subgraph
 *      It/Deit:   copies the selectionList's subgraph into a valid new location
 *                 or finds duplicates of it and removes them. these have some
 *                 important restrictions too.
 *
 * Drawing:
 *      boxprint:  draws the tree inside a box, using a recursive call to the
 *                 corresponding drawing functions of tree nodes
 *      pounce:    very similar to boxprint, except the node info themselves are
 *                 replaced with the text strings that can be typed to pounce
 *                 there (i.e. rapid movement for highlight through the
 *                 keyboard alone) - based on vim plugin EasyMotion and chrome
 *                 plugin vimium's link clicking functionality (F key)
 */
class TreeState : public QObject
{
    Q_OBJECT

public:
    /* Constructor */
    TreeState(): root(new TreeNode()), highlighted(root) {}

    /* Copy Constructor */
    //TreeState(TreeState* original):
        //root(new TreeNode(original->copyRoot())), selected(root){}

    /* Destructor */
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

    void revertSelectionList();

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
    void detachNodeAndMoveOrphans();

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
    QList< QList<TreeNode*> > popRecentChildren();

    /* Pounce */
    void setPounceIDs();
    void drawPounceTree();

    void pounceTo(QString target);

    QString getPounceString();

signals:
    void treeChanged(QString s);

private:
    TreeNode* root;

    /* Multiselect */
    QList<TreeNode*> selectionList; // All nodes selected
    QList<TreeNode*> prevSelection; // For reverting to the last selection
    TreeNode* highlighted; // Node underneath the cursor

    /* Command helpers */
    QList<TreeNode*> recentUpdatedNodes;
    QList<TreeNode*> recentParents;
    QList< QList<TreeNode*> > recentListsOfChildren; // yikes man

    /* Modification mode helper */
    QList<TreeNode*> iterationList;

    /* Visual tree */
    QString getFormattedString();
};

#endif // TREE_TREESTATE_H
