#ifndef CTREESTATEHIGHLIGHTRIGHT_H
#define CTREESTATEHIGHLIGHTRIGHT_H

#include "itreestatehighlight.h"

class CTreeStateHighlightRight : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStateHighlightRight(TreeState* t) {
        text = "Highlight right";
        tree = t; }

    /* Destructor */
    ~CTreeStateHighlightRight() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->highlightRightSibling(); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateHighlightRight(tree); }
};

#endif // CTREESTATEHIGHLIGHTRIGHT_H
