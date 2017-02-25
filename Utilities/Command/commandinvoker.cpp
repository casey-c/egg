#include "commandinvoker.h"
#include <QDebug>

/*
 * Executes a certain command
 */
void CommandInvoker::runCommand(ICommand* comm)
{
    commandStack.append(comm);
    comm->execute();
}

/*
 * Repeats the last command on the stack
 *
 * TODO: make the repeat actually a command itself, so we can undo a repetition
 * if we didn't mean to
 */
void CommandInvoker::repeatLastCommand()
{
    // Nothing to repeat
    if (commandStack.empty())
        return;

    commandStack.last()->execute();
}

/*
 * Undo
 */
void CommandInvoker::undoLastCommand()
{
    // Nothing to undo
    if (commandStack.empty())
        return;

    ICommand* command = commandStack.pop();
    command->undo();

    undoStack.append(command);
}
