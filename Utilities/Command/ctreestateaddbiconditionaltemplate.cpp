#include "ctreestateaddbiconditionaltemplate.h"

/* Adds a biconditional template */
bool CTreeStateAddBiconditionalTemplate::execute()
{
    // Add the first conditional; entry is where to put the second
    TreeNode* previous = tree->getSelected();

    first = tree->addConditionalTemplate();

    // Couldn't add the first template
    if (first == previous)
        return false;

    TreeNode* entry = first->getParent()->getParent();
    first = first->getParent();

    // Add the second
    tree->selectSpecific(entry);
    second = tree->addConditionalTemplate();
    second = second->getParent();

    tree->redraw();
    return true;
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

/* Copy */
ICommand* CTreeStateAddBiconditionalTemplate::copy()
{
    return new CTreeStateAddBiconditionalTemplate(tree);
}
