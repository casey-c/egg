#ifndef ITREESTATESELECT_H
#define ITREESTATESELECT_H

#include <QString>

#include "icommand.h"
#include "Tree/treenode.h"
#include "Tree/treestate.h"

/*
 * ITreeStateSelect specifies an interface for all commands that change the
 * selection of a tree state. These commands are similar in how they store
 * changes in selection, so they can inherit from this base class.
 */
class ITreeStateSelect : public ICommand
{
public:
    bool execute();
    void undo();

private:
    TreeState* tree;

    QList<TreeNode*> desiredSelection;
    QList<TreeNode*> previousSelection;
};

#endif // ITREESTATESELECT_H
