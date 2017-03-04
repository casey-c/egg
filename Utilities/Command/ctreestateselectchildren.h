#ifndef CTREESTATESELECTCHILDREN_H
#define CTREESTATESELECTCHILDREN_H

#include "itreestateselect.h"

class CTreeStateSelectChildren : public ITreeStateSelect
{
public:
    /* Constructor */
    CTreeStateSelectChildren(TreeState* t) {
        text = "Select children";
        tree = t; }

    /* Destructor */
    ~CTreeStateSelectChildren() {}

    /* Override the select() function of the ITreeStateSelect interface */
    void select() { tree->selectChildrenOfHighlighted(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateSelectChildren(tree); }
};

#endif // CTREESTATESELECTCHILDREN_H
