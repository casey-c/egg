#include "commandinvoker.h"
#include <QDebug>

/*
 * Destructor cleans up commands
 */
CommandInvoker::~CommandInvoker()
{
    // TODO: delete and clean commands
}

/*
 * Executes a certain command
 */
void CommandInvoker::runCommand(ICommand* comm)
{
    // Run the command, using the return value to see if it did anything
    if (comm->execute())
    {
        commandStack.append(comm);

        // Adding a new command will clear and redo-able commands
        undoStack.clear();

        updated();
    }
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

    // Run a new copy of the last command
    runCommand(commandStack.last()->copy());
    updated();
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
    updated();
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
    if (command->redo())
    {
        commandStack.append(command);
        updated();
    }
    else
    {
        // Put it back on the undo stack since nothing changed
        undoStack.append(command);
    }
}

/*
 * Updates bookkeeping on a command change
 */
void CommandInvoker::updated()
{
    QString undo = "Undo";
    QString redo = "Redo";
    QString repeat = "Repeat";

    // Undo
    bool undoable = (!commandStack.isEmpty());

    if (undoable)
    {
        // Update the undo text with the command to undo
        ICommand* command = commandStack.top();
        undo.append(": " + command->getText());
    }

    // Redo
    bool redoable = (!undoStack.isEmpty());

    if (redoable)
    {
        // Update the redo text with the command to redo
        ICommand* command = undoStack.top();
        redo.append(": " + command->getText());
    }

    // Repeat
    bool repeatable = (!commandStack.isEmpty());

    if (repeatable)
    {
        // Update the repeat text with the command to repeat
        ICommand* command = commandStack.top();
        repeat.append(": " + command->getText());
    }

    // Emit the signal
    emit updateMenu(undo,redo,repeat,undoable,redoable,repeatable);
}
