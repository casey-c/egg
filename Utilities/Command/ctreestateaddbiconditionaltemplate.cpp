#include "ctreestateaddbiconditionaltemplate.h"

/* Adds a biconditional template */
void CTreeStateAddBiconditionalTemplate::execute()
{
    // Add the first conditional; entry is where to put the second
    first = tree->addConditionalTemplate();
    TreeNode* entry = first->getParent()->getParent();
    first = first->getParent();

    // Add the second
    tree->selectSpecific(entry);
    second = tree->addConditionalTemplate();
    second = second->getParent();

    tree->redraw();
}

/* Removes the added template */
void CTreeStateAddBiconditionalTemplate::undo()
{
    tree->selectSpecific(first);
    tree->removeAndBurnTheOrphanage();

    tree->selectSpecific(second);
    tree->removeAndBurnTheOrphanage();

    tree->redraw();
}

/* Readds the template */
void CTreeStateAddBiconditionalTemplate::redo()
{
    execute();
}
