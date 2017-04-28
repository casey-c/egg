#include "cmoddeiterate.h"

CModDeiterate::CModDeiterate( TreeState* original,
                              StepOverview* over )
{
    text = "EQ: Deiterate";
    stepText = "Deiterate";
    prevTree = original;
    stepOverview = over;
}

bool CModDeiterate::modify()
{
    resultingTree = TreeState::copyTree( prevTree );
    return resultingTree->deiterate();
}
