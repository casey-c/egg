#ifndef CTREESTATEHIGHLIGHTROOT_H
#define CTREESTATEHIGHLIGHTROOT_H

#include "itreestatehighlight.h"

class CTreeStateHighlightRoot : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStateHighlightRoot(TreeState* t) {
        text = "Highlight root";
        tree = t; }

    /* Destructor */
    ~CTreeStateHighlightRoot() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->highlightRoot(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateHighlightRoot(tree); }
};

#endif // CTREESTATEHIGHLIGHTROOT_H
