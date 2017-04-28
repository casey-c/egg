#ifndef IMODIFICATION_H
#define IMODIFICATION_H

#include "icommand.h"
#include "Tree/treestate.h"
#include "ProofTree/stepoverview.h"

class IModification : public ICommand
{
public:
    bool execute();
    void undo();


protected:
    TreeState* prevTree;
    TreeState* resultingTree;

    StepOverview* stepOverview;

    virtual bool modify() = 0;
    bool redo();

    QString stepText;
};

#endif // IMODIFICATION_H
