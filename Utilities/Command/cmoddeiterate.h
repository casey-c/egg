#ifndef CMODDEITERATE_H
#define CMODDEITERATE_H

#include "imodification.h"

class CModDeiterate : public IModification
{
public:
    CModDeiterate( TreeState* original,
                   StepOverview* over );

    ICommand* copy() { return new CModDeiterate( prevTree,
                                                 stepOverview ); }

private:
    bool modify();
};

#endif // CMODDEITERATE_H
