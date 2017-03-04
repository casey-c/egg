#ifndef CTREESTATEADDCONDITIONALTEMPLATE_H
#define CTREESTATEADDCONDITIONALTEMPLATE_H

#include "itreestateadd.h"

/*
 * This command adds a conditional template to a TreeState
 */
class CTreeStateAddConditionalTemplate : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddConditionalTemplate(TreeState* t) {
        text = "Add conditional";
        tree = t; }

    /* Destructor */
    ~CTreeStateAddConditionalTemplate() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addConditionalTemplate(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddConditionalTemplate(tree); }
};

#endif // CTREESTATEADDCONDITIONALTEMPLATE_H
