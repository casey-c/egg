#include "headers/graphelement.h"
#include "headers/constants.h"

/* Factory to build a root element */
GraphElement GraphElement::Root()
{
    return GraphElement(constants::ELEMENT_ROOT,NULL,NULL);
}

/* Factory to build a cut element */
GraphElement GraphElement::Cut(GraphElement* parent)
{
    return GraphElement(constants::ELEMENT_CUT,parent,NULL);
}

/* Factory to build a statement element */
GraphElement GraphElement::Statement(GraphElement* parent, QString s)
{
    return GraphElement(constants::ELEMENT_STATEMENT, parent, s);
}

/*
 * Attempts to add the parameter as a child to this node. Only root elements or
 * cut elements are allowed to have children.
 *
 * Returns:
 * 		-1: if this node is not allowed to have children
 * 		 0: if the child is successfully added
 * 		 1: if the child already exists
 */
int GraphElement::addChild(GraphElement* child)
{
    if (isStatement())
        return -1;

    if (children.contains(child))
        return 1;

    children.push_back(child);
    return 0;
}

/*
 * Attempts to remove the child from this nodes list of children.
 *
 * Returns:
 * 		-1: if unable to remove this child
 * 		 0: if successfully removed
 */
int GraphElement::removeChild(GraphElement* child)
{
    return children.removeOne(child) ? -1 : 0;
}

/*
 * Updates this node's parent to equal the parameter
 *
 * Returns:
 * 		-1: if newParent == NULL or this is root
 * 		 0: if successfully updated
 */
int GraphElement::updateParent(GraphElement* newParent)
{
    if (newParent == NULL || isRoot())
        return -1;

    parent = newParent;
    return 0;
}
