#ifndef COMMANDINVOKER_H
#define COMMANDINVOKER_H

#include "Utilities/Command/icommand.h"
#include <QStack>
#include <QString>
#include <QObject>

/*
 * This class is responsible for keeping track of the undo / redo list and
 * serving as the master controller for all commands. When a command is created,
 * the invoker will be responsible for actually running it and keeping track of
 * which commands have been run.
 */
class CommandInvoker : public QObject
{
    Q_OBJECT

public:
    CommandInvoker() {}
    ~CommandInvoker();

    void runCommand(ICommand* comm);
    void repeatLastCommand();

    void undoLastCommand();
    void redoLastCommand();

    bool anyCommands() { return commandStack.size() > 0; }

signals:
    void updateMenu(QString undo, QString redo, QString repeat,
                    bool undoable, bool redoable, bool repeatable);

private:
    QStack<ICommand*> commandStack;
    QStack<ICommand*> undoStack;

    void updated();
};

#endif // COMMANDINVOKER_H
