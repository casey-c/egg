#ifndef ITREESTATEADD_H
#define ITREESTATEADD_H

#include <QList>

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * ITreeStateAdd specifies an interface for all commands that add something to
 * a tree state. All these commands are similar in their execute and undo
 * functions, so I have them inherit from this base class that will save on
 * repeat code.
 *
 * Commands inheriting from this interface must still specify the following:
 * Fields:
 *      text: (inherited from ICommand) - a string that describes the command
 *              for use in the undo menu. (e.g. "Undo: Add cut" will show up
 *              in the menu, instead of some generic "Undo".) This should be
 *              placed inside the constructor of the implemented command.
 *              Example contents of a constructor that sets the text:
 *
 *              text = "Add cut";
 *
 * Functions to override:
 *      copy(): (inherited from ICommand) - a function that will construct a new
 *              command of the same type, with the same parameters as the
 *              original. Example contents of an overriden copy():
 *
 *              return new CTreeStateAddCut();
 *
 *      add(): (from this interface) - a function that will add something to the
 *              tree. For example, the contents of add() can be:
 *
 *              tree->addConditionalTemplate();
 */

class ITreeStateAdd : public ICommand
{
public:
    bool execute();
    void undo();

    virtual void add() = 0;

protected:
    TreeState* tree;

    TreeNode* prevHighlighted;
    QList<TreeNode*> prevSelected;
    QList<TreeNode*> newlyCreated;
};

#endif // ITREESTATEADD_H
