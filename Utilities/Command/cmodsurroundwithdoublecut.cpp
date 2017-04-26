#include "cmodsurroundwithdoublecut.h"
#include <QDebug>

/*
 * Constructor
 */
CModSurroundWithDoubleCut::CModSurroundWithDoubleCut( TreeState* original,
                                                      StepOverview* overview )
{
    text = "Surround with double cut";
    prevTree = original;
    stepOverview = overview;
}

/*
 * Modify a copy of the tree by surrounding its highlighted node with a double
 * cut
 */
bool CModSurroundWithDoubleCut::modify()
{
    qDebug() << "Attempting to surround a copy with a double cut";

    //resultingTree = new TreeState( prevTree );
    resultingTree = TreeState::copyTree( prevTree );
    resultingTree->surroundWithDoubleCut();

    qDebug() << "Surrounded the resulting tree with double cut";

    QList<TreeNode*> changedNodes = resultingTree->popRecentNodes();

    if ( changedNodes.isEmpty() )
    {
        qDebug() << "Empty changednodes";
        return false;
    }
    else
    {
        qDebug() << "We've surrounded it with a double cut.";
        return true;
    }

}
