#include "Tree/treestate.h"
#include <QDebug>

/* TODO: implement graphstate.cpp */

/* Selects the first child of the selected region, if it exists */
void TreeState::selectAChild()
{
    if (!selected->getChildren().isEmpty())
        selected = selected->getChildren().first();

    qDebug() << "I have selected node " << selected->getID();
    emit treeChanged(getBoxedString());
}

/* Selects the parent of the selected node, if not already root. */
void TreeState::selectParent()
{
    if (!selected->isRoot())
        selected = selected->getParent();

    qDebug() << "I have selected node " << selected->getID();
    emit treeChanged(getBoxedString());
}


/* Recursively selects the parent of the selection until we reach root */
void TreeState::selectRoot()
{
    while (!selected->isRoot())
        selected = selected->getParent();

    qDebug() << "I have selected node " << selected->getID();
    emit treeChanged(getBoxedString());
}

/* Selects the previous sibling of the parent's getChildren list */
void TreeState::selectLeftSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
    {
        qDebug() << "I have selected node " << selected->getID();
        return;
    }

    /* List has the selected item and all its siblings */
    QList<TreeNode*> list = selected->getParent()->getChildren();

    TreeNode* previous = NULL;

    for (TreeNode* prev : list)
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

    qDebug() << "I have selected node " << selected->getID();
    emit treeChanged(getBoxedString());
}

/* Select the next right sibling */
void TreeState::selectRightSibling()
{
    /* Root doesn't have any siblings */
    if (selected->isRoot())
    {
        qDebug() << "I have selected node " << selected->getID();
        return;
    }

    /* List has the selected item and all its siblings */
    QList<TreeNode*> list = selected->getParent()->getChildren();

    TreeNode* next = NULL;
    QList<TreeNode*>::const_reverse_iterator it = list.rbegin();
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

    qDebug() << "I have selected node " << selected->getID();
    emit treeChanged(getBoxedString());
}

/* Adds a cut to the selected node's children */
void TreeState::addChildCut()
{
    selected = selected->addChildCut();
    qDebug() << "I have selected node " << selected->getID();
    //emit treeChanged(getFormattedString());
    emit treeChanged(getBoxedString());

}

/* Adds a double cut to the selected region */
void TreeState::addChildDoubleCut()
{
    //selected = selected->addChildCut();
    //selected = selected->addChildCut();
    addChildCut();
    addChildCut();
}

/* Adds a child statement with the string s */
void TreeState::addChildStatement(QString s)
{
    selected = selected->addChildStatement(s);
    qDebug() << "I have selected node " << selected->getID();
    //emit treeChanged(getFormattedString());

    emit treeChanged(getBoxedString());
}

/* Remove */
void TreeState::removeAndSaveOrphans()
{
    QList<TreeNode*> children = selected->getChildren();
    TreeNode* parent = selected->getParent();

    parent->addAll(children);

    // Delete the selected node
    selected->remove();

}

/* Remove the selected node and any of its children */
void TreeState::removeAndBurnTheOrphanage()
{
    selected->remove();
}

TreeState* TreeState::copyState(TreeState* currentTree)
{
    TreeNode* newRoot = TreeNode::copyTree(currentTree->getRoot());
    TreeState* newState = new TreeState(newRoot);
    return newState;
}

/* Surround with cut */
void TreeState::surroundWithCut()
{

}

/* Surround with double cut */
void TreeState::surroundWithDoubleCut()
{

}

/* Print the tree in the current state */
void TreeState::printTree()
{
    root->print(QString(""), true);
}

/* Similar to print, but stores everything in a QString */
QString TreeState::getFormattedString()
{
    return root->getFormattedString(QString(""),true);
}

/* Box print */
QString TreeState::getBoxedString()
{
    // First determine the width of inner region of box
    int width = root->getBoxWidth(0);
    qDebug() << "Final width is " << width;

    // Header (first row)
    qDebug() << "Making the top row";
    QString result = "┌";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┐\n";

    // Add each boxLine recursively
    result += root->getBoxLine(0,width,true,0,selected);

    // Footer (last row)
    result += "└";
    for (int i = 0; i < width + 2; ++i)
        result += "─";
    result += "┘";

    return result;
}

