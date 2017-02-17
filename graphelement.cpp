#include "graphelement.h"

/* TODO: implement graphelement.cpp */

static GraphElement Root()
{

}
static GraphElement Cut(GraphElement* parent)
{

}
static GraphElement Statement(GraphElement* parent, QString s)
{

}

GraphElement::GraphElement() : type(), parent(), children()
{

}

GraphElement::~GraphElement()
{

}

int GraphElement::addChild(GraphElement* child)
{

}
int GraphElement::removeChild(GraphElement* child)
{

}
QVector<GraphElement*> GraphElement::getChildren()
{

}

int GraphElement::updateParent(GraphElement* newParent)
{

}
