#include "ctreestateaddortemplate.h"

/* Adds or template */
void CTreeStateAddOrTemplate::execute()
{
    node = tree->addOrTemplate();
    node = node->getParent()->getParent();
    tree->redraw();
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
