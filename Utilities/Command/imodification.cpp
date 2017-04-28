#include "imodification.h"

/*
 * Performs a modification to the tree
 */
bool IModification::execute()
{
    // Call the child function to modify the tree
    if ( modify() )
    {
        qDebug() << "Successful modify: resultingTree is set";

        // Modify sets the resultingTree...
        // We must then put it into a ProofStep item
        // and add it to our proof

        stepOverview->addStep(stepText, resultingTree);

        return true;
    }
    else // Modification failed
    {
        return false;
    }
}

/*
 * Undoes the last modification
 */
void IModification::undo()
{
    // Removes any added ProofStep item and
    // detaches the result of the modification
    // for remaking later
}

/*
 * Reattach the resulting modification, since a simple
 * repeat isn't possible...
 * (ovveride the ICommand implementation for redo)
 */
bool IModification::redo()
{

}
