#ifndef CTREESTATEHIGHLIGHTLEFT_H
#define CTREESTATEHIGHLIGHTLEFT_H

#include "itreestatehighlight.h"

class CTreeStateHighlightLeft : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStateHighlightLeft(TreeState* t) {
        text = "Highlight left";
        tree = t;}

    /* Destructor */
    ~CTreeStateHighlightLeft() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->highlightLeftSibling(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateHighlightLeft(tree); }
};

#endif // CTREESTATEHIGHLIGHTLEFT_H
