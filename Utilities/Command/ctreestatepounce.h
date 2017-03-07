#ifndef CTREESTATEPOUNCE_H
#define CTREESTATEPOUNCE_H

#include "itreestatehighlight.h"

class CTreeStatePounce : public ITreeStateHighlight
{
public:
    /* Constructor */
    CTreeStatePounce(TreeState* t, QString tar) : target(tar) {
        text = "Pounce to " + target;
        tree = t; }

    /* Destructor */
    ~CTreeStatePounce() {}

    /* Override the highlight() function of the ITreeStateHighlight interface */
    void highlight() { tree->pounceTo(target); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStatePounce(tree, target); }
private:
    QString target;
};
#endif // CTREESTATEPOUNCE_H
