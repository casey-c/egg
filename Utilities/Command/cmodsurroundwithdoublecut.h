#ifndef CMODSURROUNDWITHDOUBLECUT_H
#define CMODSURROUNDWITHDOUBLECUT_H

#include "imodification.h"
#include "Tree/treestate.h"
#include "ProofTree/stepoverview.h"

class CModSurroundWithDoubleCut : public IModification
{
public:
    CModSurroundWithDoubleCut( TreeState* original,
                               StepOverview* over );

    ICommand* copy() { return new CModSurroundWithDoubleCut( prevTree,
                                                             stepOverview ); }
private:
    bool modify();
};

#endif // CMODSURROUNDWITHDOUBLECUT_H
