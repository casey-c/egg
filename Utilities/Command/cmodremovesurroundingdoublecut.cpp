#include "cmodremovesurroundingdoublecut.h"

CModRemoveSurroundingDoubleCut::CModRemoveSurroundingDoubleCut( TreeState* original,
                                                                StepOverview* overview )
{
    text = "EQ: Remove surrounding double cut";
    stepText = "Remove surrounding double cut";
    prevTree = original;
    stepOverview = overview;
}

bool CModRemoveSurroundingDoubleCut::modify()
{
    qDebug() << "Attempting to remove a surrounding double cut";

    resultingTree = TreeState::copyTree( prevTree );
    resultingTree->removeSurroundingDoubleCut();

    QList<TreeNode*> changedNodes = resultingTree->popRecentNodes();
    if ( changedNodes.empty() )
    {
        qDebug() << "No changed nodes!";
        return false;
    }
    else
    {
        qDebug() << "Changed nodes not empty";
        return true;
    }
}
