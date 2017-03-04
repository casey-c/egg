#ifndef CTREESTATEADDSTATEMENT_H
#define CTREESTATEADDSTATEMENT_H

#include <QDebug>
#include "itreestateadd.h"

/*
 * This command will add a statement to a tree state.
 */
class CTreeStateAddStatement : public ITreeStateAdd
{
public:
    /* Constructor */
    CTreeStateAddStatement(TreeState* t, QString s) : statement(s) {
        qDebug() << "Adding statement " << statement;
        text = "Add " + statement;
        tree = t; }

    /* Destructor */
    ~CTreeStateAddStatement() {}

    /* Override the add() function of the ITreeStateAdd interface */
    void add() { tree->addChildStatement(statement); }

    /* Override the copy() function of the ICommand interface */
    ICommand* copy() { return new CTreeStateAddStatement(tree,statement); }

private:
    QString statement;
};

#endif // CTREESTATEADDSTATEMENT_H
