#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include "Utilities/Command/icommand.h"
#include <QStack>

/*
 * This class is responsible for keeping track of the undo / redo list and
 * serving as the master controller for all commands. When a command is created,
 * the invoker will be responsible for actually running it and keeping track of
 * which commands have been run.
 */
class CommandInvoker
{
public:
    CommandInvoker() {}

    void runCommand(ICommand* comm);
    void repeatLastCommand();

    void undoLastCommand();

private:
    QStack<ICommand*> commandStack;
    QStack<ICommand*> undoStack;
};

#endif // COMMANDINVOKER_H
