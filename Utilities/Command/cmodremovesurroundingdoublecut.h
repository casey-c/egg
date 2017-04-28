#ifndef CMODREMOVESURROUNDINGDOUBLECUT_H
#define CMODREMOVESURROUNDINGDOUBLECUT_H

#include "imodification.h"

class CModRemoveSurroundingDoubleCut : public IModification
{
public:
    CModRemoveSurroundingDoubleCut( TreeState* original,
                                    StepOverview* over );

    ICommand* copy() { return new CModRemoveSurroundingDoubleCut( prevTree,
                                                                  stepOverview ); }
private:
    bool modify();
};

#endif // CMODREMOVESURROUNDINGDOUBLECUT_H
