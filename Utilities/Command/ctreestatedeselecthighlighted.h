#ifndef CTREESTATEDESELECTHIGHLIGHTED_H
#define CTREESTATEDESELECTHIGHLIGHTED_H

#include "itreestateselect.h"

class CTreeStateDeselectHighlighted : public ITreeStateSelect
{
public:
    /* Constructor */
    CTreeStateDeselectHighlighted(TreeState* t) {
        text = "Deselect";
        tree = t; }

    /* Destructor */
    ~CTreeStateDeselectHighlighted() {}

    /* Override the select() function of the ITreeStateSelect interface */
    void select() { tree->deselectHighlighted(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateDeselectHighlighted(tree); }
};

#endif // CTREESTATEDESELECTHIGHLIGHTED_H
