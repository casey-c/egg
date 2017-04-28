#ifndef CMODITERATE_H
#define CMODITERATE_H

#include "imodification.h"

class CModIterate : public IModification
{
public:
    CModIterate( TreeState* original,
                 StepOverview* over );

    ICommand* copy() { return new CModIterate( prevTree,
                                               stepOverview ); }

private:
    bool modify();
};

#endif // CMODITERATE_H
