#include "cmoderasure.h"

CModErasure::CModErasure( TreeState* original,
                          StepOverview* over )
{
    text = "INF: Erasure";
    stepText = "Erasure";
    prevTree = original;
    stepOverview = over;
}

bool CModErasure::modify()
{
    resultingTree = TreeState::copyTree( prevTree );
    return resultingTree->erasure();
}
