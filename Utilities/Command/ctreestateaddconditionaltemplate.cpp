#include "ctreestateaddconditionaltemplate.h"

/* Adds conditional template */
bool CTreeStateAddConditionalTemplate::execute()
{
    TreeNode* previous = tree->getSelected();
    node = tree->addConditionalTemplate();

    // No change
    if (previous == node)
        return false;

    node = node->getParent();
    tree->redraw();
    return true;
}

/* Removes the added template */
void CTreeStateAddConditionalTemplate::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Copy */
ICommand* CTreeStateAddConditionalTemplate::copy()
{
    return new CTreeStateAddConditionalTemplate(tree);
}
