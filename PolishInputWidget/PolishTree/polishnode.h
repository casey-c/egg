#ifndef POLISHNODE_H
#define POLISHNODE_H

#include <QString>
#include <QList>

/*
 * A polish node is a node on a polish tree. Polish notation is operator-first,
 * and details second. The benefits of this prefix notation is that WFF's are
 * easy to create and don't need any parentheses management. Further, polish
 * notation trees can easily be converted into their existential graph
 * equivalents.
 *
 * Most of the processing for these manipulations are inclued in the
 * PolishInputWidget itself. This class stores connections and details (what
 * gets displayed by the node itself) to assist in construction of the tree.
 *
 * This implementation is essentially a port of Casey's other MIT-licensed
 * project, logic-builder, which can be found at:
 *
 * https://github.com/casey-c/logic-builder/
 *
 * This port only includes features relative to existential graphs, namely it
 * only includes basic propositional logic (no quantifiers, formulas, or
 * constants).
 *
 * Some things to consider:
 *      * wrapped bool determines whether or not this node/branch of the tree
 *              should be wrapped in parentheses. All multiple arity operators
 *              become wrapped if their direct parent is also an operator. This
 *              essentially occurs anytime operators get nested, but there's
 *              some logic to avoid wrapping on the outermost (root) operator if
 *              it can be avoided.
 *      * nodes are created through the factory constructor (which makes a root)
 *              and through the makeNewChild(). When first created, nodes are
 *              devoid of any details. This is because the details are set when
 *              they are popped from the stack in the input widget. Nodes are
 *              created as placeholders to be filled with meaning later on, and
 *              when the stack empties, we have a WFF.
 */

class PolishNode
{
public:
    /* Factory to make a root */
    static PolishNode* makeRoot(QString t, bool o)
        { return new PolishNode(nullptr, t, o, false); }

    /* Destructor */
    ~PolishNode();

    /* Create and update new nodes */
    PolishNode* makeNewChild();
    void setDetails(QString t, bool o);

    /* Print */
    QString toPlaintext();
    QString getText() { return text; }

private:
    /* Private constructor */
    PolishNode(PolishNode* p, QString t, bool o, bool w) :
        parent(p),
        text(t),
        op(o),
        wrapped(w) {}

    /* Connections */
    PolishNode* parent;
    QList<PolishNode*> children;

    /* Details */
    QString text;
    bool op;
    bool wrapped;
};

#endif // POLISHNODE_H
