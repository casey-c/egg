#include "graphstate.h"

/* TODO: implement graphstate.cpp */

/* Destructor */
GraphState::~GraphState()
{

}

/* Selects the first child of the selected region, if it exists */
void GraphState::selectAChild()
{
    if (!selected->getChildren().isEmpty())
        selected = selected->getChildren().first();
}

/* Selects the parent of the selected node, if not already root. */
void GraphState::selectParent()
{
    if (!selected->isRoot())
        selected = selected->getParent();
}

/* Recursively selects the parent of the selection until we reach root */
void GraphState::selectRoot()
{
    while (!selected->isRoot())
        selected = selected->getParent();
}

/* Selects the previous sibling of the parent's getChildren list */
void GraphState::selectLeftSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
        return;

    /* List has the selected item and all its siblings */
    QList<GraphElement*> list = selected->getParent()->getChildren();

    GraphElement* previous = NULL;

    for (GraphElement* prev : list)
    {
        if (prev == selected)
            break;
        previous = prev;
    }

    /* Nothing to the left */
    if (previous == NULL)
       selected = list.last();
    else /* Choose the previous as the new selection */
        selected = previous;
}

/* Select the next right sibling */
void GraphState::selectRightSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
        return;

    /* List has the selected item and all its siblings */
    QList<GraphElement*> list = selected->getParent()->getChildren();

    GraphElement* next = NULL;
    QList<GraphElement*>::const_reverse_iterator it = list.rbegin();
    for (; it != list.rend(); ++it)
    {
        if (*it == selected)
            break;
        next = *it;
    }

    /* Nothing to the right */
    if (next == NULL)
       selected = list.front();
    else /* Choose the next as the new selection */
        selected = next;
}


/* Add */
void GraphState::addChildCut()
{

}
void GraphState::addChildDoubleCut()
{

}
void GraphState::addChildStatement(QString s)
{

}

/* Remove */
void GraphState::removeAndSaveOrphans()
{

}
void GraphState::removeAndBurnTheOrphanage()
{

}

/* Surround with cut */
void GraphState::surroundWithCut()
{

}
void GraphState::surroundWithDoubleCut()
{

}
