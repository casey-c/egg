#ifndef CTREESTATEREVERTSELECTION_H
#define CTREESTATEREVERTSELECTION_H

#include "itreestateselect.h"

class CTreeStateRevertSelection : public ITreeStateSelect
{
public:
    /* Constructor */
    CTreeStateRevertSelection(TreeState* t) {
        text = "Revert selection";
        tree = t; }

    /* Destructor */
    ~CTreeStateRevertSelection() {}

    /* Override the select() function of the ITreeStateSelect interface */
    void select() { tree->revertSelectionList(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateRevertSelection(tree); }
};

#endif // CTREESTATEREVERTSELECTION_H
