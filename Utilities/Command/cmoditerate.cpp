#include "cmoditerate.h"

CModIterate::CModIterate( TreeState* original,
                          StepOverview* over )
{
    text = "EQ: Iterate";
    stepText = "Iterate";
    prevTree = original;
    stepOverview = over;
}

bool CModIterate::modify()
{
    resultingTree = TreeState::copyTree( prevTree );
    return resultingTree->iterate();
}
