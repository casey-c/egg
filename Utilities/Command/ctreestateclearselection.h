#ifndef CTREESTATECLEARSELECTION_H
#define CTREESTATECLEARSELECTION_H

#include "itreestateselect.h"

class CTreeStateClearSelection : public ITreeStateSelect
{
public:
    /* Constructor */
    CTreeStateClearSelection(TreeState* t) {
        text = "Clear selection";
        tree = t; }

    /* Destructor */
    ~CTreeStateClearSelection() {}

    /* Override the select() function of the ITreeStateSelect interface */
    void select() { tree->clearSelection(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateClearSelection(tree); }
};

#endif // CTREESTATECLEARSELECTION_H
