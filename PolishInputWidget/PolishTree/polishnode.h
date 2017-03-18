#ifndef POLISHNODE_H
#define POLISHNODE_H

#include <QString>
#include <QList>

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
