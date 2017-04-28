#ifndef CMODERASURE_H
#define CMODERASURE_H

#include "imodification.h"

class CModErasure : public IModification
{
public:
    CModErasure( TreeState* original,
                 StepOverview* over );

    ICommand* copy() { return new CModErasure(prevTree,
                                              stepOverview); }

private:
    bool modify();
};

#endif // CMODERASURE_H
