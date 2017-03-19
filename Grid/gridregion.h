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

    /* Print */
    QString toString();

private:
    /* Connections */
    GridRegion* parent;
    QList<GridRegion*> children;
    TreeNode* associatedNode;

    /* Array */
    int width;
    int height;
    GridCell*** array; // array[col][row]

    /* Copies my elements into the provided array */
    void transferCells(int sx, int sy, GridCell*** parentArray);

    /* Helper to handle array allocations:
     * Only call after setting width and height */
    void allocateSpace();

    /* Helper to fill the corners and borders of a cut
     * Only call after allocating space */
    void addBorders();

    /* Helper to fill in padding
     * Only call after allocating space and transfering all children*/
    void fillPadding();
};

#endif // GRIDREGION_H
