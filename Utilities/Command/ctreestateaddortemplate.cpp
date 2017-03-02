#include "ctreestateaddortemplate.h"

/* Adds or template */
bool CTreeStateAddOrTemplate::execute()
{
    TreeNode* previous = tree->getSelected();
    node = tree->addOrTemplate();

    // No change
    if (node == previous)
        return false;

    node = node->getParent();

    tree->redraw();
    return true;
}

/* Removes the added template */
void CTreeStateAddOrTemplate::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the template */
void CTreeStateAddOrTemplate::redo()
{
    execute();
}

/* Copy */
ICommand* CTreeStateAddOrTemplate::copy()
{
    return new CTreeStateAddOrTemplate(tree);
}
