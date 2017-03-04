#ifndef CTREESTATEADDDOUBLECUT_H
#define CTREESTATEADDDOUBLECUT_H

#include "itreestateadd.h"

/*
 * This command adds a double cut to a TreeState
 */
class CTreeStateAddDoubleCut : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddDoubleCut(TreeState* t) {
        text = "Add double cut";
        tree = t; }

    /* Destructor */
    ~CTreeStateAddDoubleCut() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addChildDoubleCut(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddDoubleCut(tree); }
};


#endif // CTREESTATEADDDOUBLECUT_H
