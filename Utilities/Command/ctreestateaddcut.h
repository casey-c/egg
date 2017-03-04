#ifndef CTREESTATEADDCUT_H
#define CTREESTATEADDCUT_H

#include "itreestateadd.h"

/*
 * This command adds a cut to a TreeState
 */
class CTreeStateAddCut : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddCut(TreeState* t) {
        text = "Add cut";
        tree = t; }

    /* Destructor */
    ~CTreeStateAddCut() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addChildCut(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddCut(tree); }
};

#endif // CTREESTATEADDCUT_H
