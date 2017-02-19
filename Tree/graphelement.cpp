#include "headers/graphelement.h"
#include "headers/constants.h"

/* Add child cut */
GraphElement* GraphElement::addChildCut()
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_CUT;
        parent->placeHolderChild = false;
        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_CUT;
        placeHolderChild = false;

        return placeholder;
    }

    // Otherwise, make a new cut element
    //GraphElement newCut(constants::ELEMENT_CUT,this,NULL);
    GraphElement* newCut = new GraphElement(constants::ELEMENT_CUT,this,NULL);
    children.append(newCut);
    return newCut;
}

/* Add child statement */
GraphElement* GraphElement::addChildStatement(QString s)
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is a placeholder
    if (isPlaceHolder())
    {
        // Replace this element
        type = constants::ELEMENT_STATEMENT;
        name = s;
        parent->placeHolderChild = false;

        return this;
    }

    // See if we have a placeholder child to replace
    if (placeHolderChild)
    {
        // Replace that element
        placeholder->type = constants::ELEMENT_STATEMENT;
        placeholder->name = s;
        placeHolderChild = false;

        return placeholder;
    }

    // Otherwise, make a new statement element
    GraphElement* newStatement = new GraphElement(constants::ELEMENT_STATEMENT,this,s);
    children.append(newStatement);

    return newStatement;

}

/* Add child placeholder */
GraphElement* GraphElement::addChildPlaceholder()
{
    // Check if this is allowed to have children
    if (isStatement())
        return this;

    // Check if this is already a placeholder
    if (isPlaceHolder())
        return this;

    // Check if we already have a child placeholder
    if (placeHolderChild)
        return this;

    // We should be good to add a new child placeholder
    GraphElement* newPlaceholder = new GraphElement(constants::ELEMENT_PLACEHOLDER,this,NULL);
    placeHolderChild = true;
    placeholder = newPlaceholder;

    return newPlaceholder;
}
