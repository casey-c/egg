#include "itreestatehighlight.h"

/*
 * Highlights the desired node
 */
bool ITreeStateHighlight::execute()
{
    // Store the previously highlighted node
    previousHighlighted = tree->getHighlighted();

    // Highlight the desired node on the tree
    highlight();

    // No change in highlighting
    if (tree->getHighlighted() == previousHighlighted)
        return false;

    tree->redraw();
    return true;
}

/*
 * Reverts to the previously highlighted node
 */
void ITreeStateHighlight::undo()
{
    tree->highlightSpecific(previousHighlighted);
    tree->redraw();
}
