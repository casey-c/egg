#ifndef CMODADDEMPTYDOUBLECUT_H
#define CMODADDEMPTYDOUBLECUT_H

#include "imodification.h"

class CModAddEmptyDoubleCut : public IModification
{
public:
    CModAddEmptyDoubleCut( TreeState* original,
                           StepOverview* over );

    ICommand* copy() { return new CModAddEmptyDoubleCut( prevTree,
                                                         stepOverview ); }

private:
    bool modify();
};

#endif // CMODADDEMPTYDOUBLECUT_H
