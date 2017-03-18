#ifndef CTREESTATEADDORTEMPLATE_H
#define CTREESTATEADDORTEMPLATE_H

#include "itreestateadd.h"

/*
 * This command adds an or template to a Tree State
 */
class CTreeStateAddOrTemplate : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddOrTemplate(TreeState* t) {
        text = "Add OR template";
        tree = t;}

    /* Destructor */
    ~CTreeStateAddOrTemplate() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addOrTemplate(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddOrTemplate(tree); }
};

#endif // CTREESTATEADDORTEMPLATE_H
