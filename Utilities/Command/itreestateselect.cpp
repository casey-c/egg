#include "itreestateselect.h"

/*
 * Selects something on the tree state
 */
bool ITreeStateSelect::execute()
{
    previousSelection = tree->getSelectionList();

    // Call the overriden function to perform the actual selection
    select();

    // Nothing changed
    if (previousSelection == tree->getSelectionList())
        return false;

    tree->redraw();
    return true;
}

/*
 * Reverts to the previous selected state
 */
void ITreeStateSelect::undo()
{
    tree->setSelectionList(previousSelection);
    tree->redraw();
}
