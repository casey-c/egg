#include "commandinvoker.h"
#include <QDebug>

/*
 * Executes a certain command
 */
void CommandInvoker::runCommand(ICommand* comm)
{
    commandStack.append(comm);
    comm->execute();

    // Adding a new command will clear and redo-able commands
    undoStack.clear();
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
 * Performs an undo on the last added command
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

/*
 * Performs a redo on the last undone command
 */
void CommandInvoker::redoLastCommand()
{
    // Nothing to redo
    if (undoStack.empty())
        return;

    ICommand* command = undoStack.pop();
    command->redo();

    commandStack.append(command);
}
