#include "ctreestateaddconditionaltemplate.h"

/* Adds conditional template */
void CTreeStateAddConditionalTemplate::execute()
{
    node = tree->addConditionalTemplate();
    node = node->getParent()->getParent();
    tree->redraw();
}

/* Removes the added template */
void CTreeStateAddConditionalTemplate::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the template */
void CTreeStateAddConditionalTemplate::redo()
{
    execute();
}
