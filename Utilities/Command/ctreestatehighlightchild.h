#ifndef CTREESTATEHIGHLIGHTCHILD_H
#define CTREESTATEHIGHLIGHTCHILD_H

#include "itreestatehighlight.h"

class CTreeStateHighlightChild : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStateHighlightChild(TreeState* t) {
        text = "Highlight child";
        tree = t; }

    /* Destructor */
    ~CTreeStateHighlightChild() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->highlightChild(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateHighlightChild(tree); }
};

#endif // CTREESTATEHIGHLIGHTCHILD_H
