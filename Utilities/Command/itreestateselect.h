#ifndef ITREESTATESELECT_H
#define ITREESTATESELECT_H

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * ITreeStateSelect is an interface for commands that wish to change the
 * selected elements of a tree state.
 *
 * Commands inheriting from this interface must still specify the following:
 * Fields:
 *      text: (inherited from ICommand) - a string that describes the command
 *              for use in the undo menu. (e.g. "Undo: Clear selection" will
 *              show up in the menu, instead of some generic "Undo".) This
 *              should be placed inside the constructor of the implemented
 *              command. Example contents of a constructor that sets the text:
 *
 *              text = "Clear selection";
 *
 * Functions to override:
 *      copy(): (inherited from ICommand) - a function that will construct a new
 *              command of the same type, with the same parameters as the
 *              original. Example contents of an overriden copy():
 *
 *              return new CTreeStateClearSelection();
 *
 *      select(): (frome this interface) - a function that will call the
 *              appropriate select function on the tree. For example:
 *
 *              tree->clearSelection();
 */
class ITreeStateSelect : public ICommand
{
public:
    bool execute();
    void undo();

    virtual void select() = 0;

protected:
    TreeState* tree;

    QList<TreeNode*> previousSelection;
};

#endif // ITREESTATESELECT_H
