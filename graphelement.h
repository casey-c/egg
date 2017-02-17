#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H

#include<QList>
#include <QString>

#include "constants.h"

/*
 * A GraphElement is one of the particular objects of an existential graph tree.
 * It can either be a root element, a cut element, or a statement. The first
 * two are allowed to have child elements, while the statement cannot. Statements
 * also have a particular string associated with them (e.g. the letter 'A')
 * that will be displayed by the canvas.
 *
 * Graph elements are mutable: at any time, root elements and cut elements can
 * add or remove child graph elements. Cut elements and statements can update
 * their parent elements as well. Root elements, once created, are intended
 * to stay the root elements for a particular graph state for the lifespan of
 * that particular graph state. Further, only one root element is allowed for
 * each graph state (see graphstate.h for details).
 *
 * This class uses the named constructors idiom: this makes it more intuitive to
 * create a GraphElement of a certain type, at the cost of needing to call a
 * static method instead of a constructor.
 *
 * Creation is handled through the three named static methods, which calls the
 * private constructor and creates the corresponding GraphElement of the proper
 * type.
 *
 * Example usage:
 * GraphElement root = GraphElement::Root();
 * GraphElement cut = GraphElement::Cut(root);
 * GraphElement statement = GraphElement::Statement(cut, QString("A"));
 */

class GraphElement
{
public:
    static GraphElement Root();
    static GraphElement Cut(GraphElement* parent);
    static GraphElement Statement(GraphElement* parent, QString s);

    ~GraphElement() {}

    int addChild(GraphElement* child);
    int removeChild(GraphElement* child);

    int updateParent(GraphElement* newParent);

    /* Boolean identifiers */
    bool isRoot() { return type == constants::ELEMENT_ROOT; }
    bool isCut() { return type == constants::ELEMENT_CUT; }
    bool isStatement(){ return type == constants::ELEMENT_STATEMENT; }

private:
    /* Private constructor forces factory method construction */
    GraphElement(const int t, GraphElement* p, QString n)
        : type(t)
        , parent(p)
        , name(n) {}

    const int type;
    GraphElement* parent;
    QList<GraphElement*> children;
    QString name;

};

#endif // GRAPHELEMENT_H
