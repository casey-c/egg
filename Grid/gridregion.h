#ifndef GRIDREGION_H
#define GRIDREGION_H

#include "gridcell.h"
#include "Tree/treenode.h"

class GridCell;

class GridRegion
{
public:
    /* Constructor */
    GridRegion(TreeNode* n) : associatedNode(n) {}

    /* Make new nodes */
    GridRegion* addChildRegion(TreeNode* n);

    /* Where the magic happens */
    void buildGrid();

    QList<GridRegion*> getChildren() { return children; }

private:
    /* Connections */
    GridRegion* parent;
    QList<GridRegion*> children;
    TreeNode* associatedNode;

    /* Array */
    int width;
    int height;
    GridCell** array;

    /* Copies my elements into the provided array */
    void transferCells(int sx, int sy, GridCell** array);
};

#endif // GRIDREGION_H
