#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QDebug>

/*
 * ICommand specifies the interface for concrete commands to implement.
 *
 * All commands that wish to be handled by the command invoker must extend from
 * this abstract class.
 */

class ICommand
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

#endif // ICOMMAND_H
