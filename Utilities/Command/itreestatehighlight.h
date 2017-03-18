#ifndef ITREESTATEHIGHLIGHT_H
#define ITREESTATEHIGHLIGHT_H

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * ITreeStateHighlight is an interface for commands that wish to change the
 * current highlighted node of a tree state.
 *
 * Commands inheriting from this interface must still specify the following:
 * Fields:
 *      text: (inherited from ICommand) - a string that describes the command
 *              for use in the undo menu. (e.g. "Undo: Highlight root" will show
 *              up in the menu, instead of some generic "Undo".) This should be
 *              placed inside the constructor of the implemented command.
 *              Example contents of a constructor that sets the text:
 *
 *              text = "Select root";
 *
 * Functions to override:
 *      copy(): (inherited from ICommand) - a function that will construct a new
 *              command of the same type, with the same parameters as the
 *              original. Example contents of an overriden copy():
 *
 *              return new CTreeStateHighlightRoot();
 *
 *      highlight(): (from this interface) - a function that will call the
 *              appropriate highlight function on the tree. For example:
 *
 *              tree->highlightRoot();
 */
class ITreeStateHighlight : public ICommand
{
public:
    bool execute();
    void undo();

    virtual void highlight() = 0;

protected:
    TreeState* tree;
    TreeNode* previousHighlighted;
};

#endif // ITREESTATEHIGHLIGHT_H
