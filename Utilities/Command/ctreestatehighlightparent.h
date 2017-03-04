#ifndef CTREESTATEHIGHLIGHTPARENT_H
#define CTREESTATEHIGHLIGHTPARENT_H

#include "itreestatehighlight.h"

class CTreeStateHighlightParent : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStateHighlightParent(TreeState* t) {
        text = "Highlight parent";
        tree = t; }

    /* Destructor */
    ~CTreeStateHighlightParent() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->highlightParent(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateHighlightParent(tree); }
};

#endif // CTREESTATEHIGHLIGHTPARENT_H
