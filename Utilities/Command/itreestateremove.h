#ifndef ITREESTATEREMOVE_H
#define ITREESTATEREMOVE_H

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"

#include <QList>

/*
 * ITreeStateRemove is an interface for commands that wish to remove nodes from
 * a tree state.
 *
 * Commands inheriting from this interface must still specify the following:
 * Fields:
 *      text: (inherited from ICommand) - a string that describes the command
 *              for use in the undo menu. (e.g. "Undo: Remove node" will show
 *              up in the menu, instead of some generic "Undo".) This should be
 *              placed inside the constructor of the implemented command.
 *              Example contents of a constructor that sets the text:
 *
 *              text = "Remove node";
 *
 * Functions to override:
 *      copy(): (inherited from ICommand) - a function that will construct a new
 *              command of the same type, with the same parameters as the
 *              original. Example contents of an overriden copy():
 *
 *              return new CTreeStateRemoveNode();
 *
 *      remove(): (from this interface) - a function that will call the
 *              appropriate remove function on the tree. For example:
 *
 *              tree->removeAndBurnTheOrphanage(TreeNode* node);
 */
class ITreeStateRemove : public ICommand
{
public:
    bool execute();
    void undo();

    virtual void remove() = 0;

protected:
    TreeState* tree;
    TreeNode* prevHighlighted;

    QList<TreeNode*> prevSelected;
    QList<TreeNode*> removedNodes;
    QList<TreeNode*> oldParents;
};

#endif // ITREESTATEREMOVE_H
