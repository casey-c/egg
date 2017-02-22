#ifndef ICOMMAND_H
#define ICOMMAND_H

/*
 * ICommand specifies the interface for concrete commands to implement.
 */

class ICommand
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;
};

#endif // ICOMMAND_H
