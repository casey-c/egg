#ifndef CTREESTATEADDBICONDITIONALTEMPLATE_H
#define CTREESTATEADDBICONDITIONALTEMPLATE_H

#include "itreestateadd.h"

/*
 * This command adds a biconditional template to a TreeState
 */
class CTreeStateAddBiconditionalTemplate : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddBiconditionalTemplate(TreeState* t) {
        text = "Add biconditional";
        tree = t; }

    /* Destructor */
    ~CTreeStateAddBiconditionalTemplate() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addBiconditionalTemplate(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddBiconditionalTemplate(tree); }
};

#endif // CTREESTATEADDBICONDITIONALTEMPLATE_H
