#include "ctreestateaddbiconditionaltemplate.h"

/* Adds a biconditional template */
void CTreeStateAddBiconditionalTemplate::execute()
{
    node = tree->addBiConditionalTemplate();
    node = node->getParent()->getParent();
    tree->redraw();
}

/* Removes the added template */
void CTreeStateAddBiconditionalTemplate::undo()
{
    tree->selectSpecific(node);
    tree->removeAndBurnTheOrphanage();
    tree->redraw();
}

/* Readds the template */
void CTreeStateAddBiconditionalTemplate::redo()
{
    execute();
}
