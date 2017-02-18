#ifndef GRAPHELEMENT_H
#define GRAPHELEMENT_H

#include<QList>
#include <QString>

#include "constants.h"

/*
 * A GraphElement is one of the particular objects of an existential graph tree.
 * It can either be a root element, a cut element, a statement, or a
 * placeholder. Root elements and cut elements can have child GraphElements that
 * grow the tree beneath them. Statements also have a particular string
 * associated with them (e.g. the letter 'A') that will be displayed by the
 * canvas.
 *
 * Placeholder elements are special: when drawn on a canvas, they are the same
 * size as a statement, but are a selectable empty region. They are used for
 * graphically saving space for "template" insertions: for example, adding a
 * template for an implication will draw an outer cut and an inner cut. The
 * actual implication object will have objects inside the outer region imply
 * everything inside the inner region. Without placeholders to specify saved
 * space for the antecedent, adding the implication template will be identical
 * to adding a double cut template, which is not what we want to show.
 *
 * Placeholder elements have some unique functionality: they can mutate their
 * type from placeholder to either cut or statement. This can be done by calling
 * the addChild() function when the placeholder is selected; this essentially
 * "replaces" the placeholder into an actual graph element. Additionally,
 * placeholder elements are the first to be replaced when the parent region is
 * selected and calls addChild(). For this to happen smoothly, a bool is set
 * to keep track of whether any GraphElement has a placeholder child; if
 * addChild() is called on such an element, the backend will make sure that the
 * placeholder is updated to whichever element is desired.
 *
 * Graph elements are mutable in terms of what they can point to: at any time,
 * root elements and cut elements can add or remove child graph elements. Cut
 * elements and statements can update their parent elements as well. Root
 * elements, once created, are intended to stay the root elements for a
 * particular graph state for the lifespan of that particular graph state.
 * Further, only one root element is allowed for each graph state (see
 * graphstate.h for details).
 *
 * Only placeholder elements can mutate their type: the other three types are
 * permanent once set.
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
 *
 * These factories are also responsible to determining if it needs to
 * replace a placeholder region, and computes the required steps if needed.
 */

class GraphElement
{
public:
    static GraphElement Root();
    static GraphElement Cut(GraphElement* parent);
    static GraphElement Statement(GraphElement* parent, QString s);
    static GraphElement PlaceHolder(GraphElement* parent);

    ~GraphElement() {}

    int addChild(GraphElement* child);
    int removeChild(GraphElement* child);

    /* TODO: fix rep. exposure (make private? friend graphstate?) */
    QList<GraphElement*> getChildren() { return children; }
    GraphElement* getParent() { return parent; }

    int updateParent(GraphElement* newParent);

    /* Boolean identifiers */
    bool isRoot() { return type == constants::ELEMENT_ROOT; }
    bool isCut() { return type == constants::ELEMENT_CUT; }
    bool isStatement(){ return type == constants::ELEMENT_STATEMENT; }

    bool isPlaceHolder() { return type == constants::ELEMENT_PLACEHOLDER; }
    bool hasPlaceHolderChild() { return placeHolderChild == true; }

private:
    /* Private constructor forces factory method construction */
    GraphElement(const int t, GraphElement* p, QString n)
        : type(t)
        , parent(p)
        , name(n)
        , placeHolderChild() {}

    const int type;
    GraphElement* parent;
    QList<GraphElement*> children;
    QString name;

    bool placeHolderChild;
};

#endif // GRAPHELEMENT_H
