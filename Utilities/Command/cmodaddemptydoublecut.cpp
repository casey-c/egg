#include "cmodaddemptydoublecut.h"

CModAddEmptyDoubleCut::CModAddEmptyDoubleCut( TreeState* original,
                                              StepOverview* over )
{
    text = "EQ: Add empty double cut";
    stepText = "Add double cut";
    prevTree = original;
    stepOverview = over;
}

bool CModAddEmptyDoubleCut::modify()
{
    resultingTree = TreeState::copyTree( prevTree );
    resultingTree->addChildDoubleCut();

    QList<TreeNode*> changedNodes = resultingTree->popRecentNodes();

    if ( changedNodes.empty() )
        return false;
    else
        return true;
}
