#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <QDebug>
#include <QString>

/*
 * ICommand specifies the interface for concrete commands to implement.
 *
 * All commands that wish to be handled by the command invoker must extend from
 * this abstract class.
 */

class ICommand
{

public:
    virtual bool execute() = 0;
    virtual void undo() = 0;
    virtual bool redo() { return execute(); }

    virtual ICommand* copy() = 0;

    QString getText() { return text; }

protected:
    QString text;

};

#endif // ICOMMAND_H
