#ifndef CTREESTATESELECTHIGHLIGHTED_H
#define CTREESTATESELECTHIGHLIGHTED_H

#include "itreestateselect.h"

class CTreeStateSelectHighlighted : public ITreeStateSelect
{
public:
    /* Constructor */
    CTreeStateSelectHighlighted(TreeState* t) {
        text = "Select";
        tree = t; }

    /* Destructor */
    ~CTreeStateSelectHighlighted() {}

    /* Override the select() function of the ITreeStateSelect interface */
    void select() { tree->selectHighlighted(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateSelectHighlighted(tree); }
};

#endif // CTREESTATESELECTHIGHLIGHTED_H
