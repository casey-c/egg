#ifndef GRIDREGION_H
#define GRIDREGION_H

#include "gridcell.h"
#include "Tree/treenode.h"

// Forward declaration of GridCells
class GridCell;

/*
 * A GridRegion is a wrapper of a 2-D array of GridCells. GridRegions point to
 * the specific TreeNode that they are associated with. They also maintain a
 * tree structure inside the array itself - this enables GridRegions to be
 * comprised of other GridRegions, with overlapping GridCells that point to
 * correct parent.
 *
 * Regions are initially created in the Grid class as a conversion from a
 * TreeState. Once allocated, they are ordered such that child regions are made
 * before their parents, so that GridCells can be shared between them and that
 * the GridCells belong to the most specific region that it needs to be.
 *
 * Filling the GridRegion with meaningful information happens in the buildGrid()
 * method. Here, the region determines the size of its array, allocates the
 * necessary space to hold the GridCells, fills any cell with information needed
 * from its children, and finally fills in the rest of the details that it needs
 * to fill.
 *
 * On completion of the buildGrid() method, the region is completely built and
 * all cells in its array are non-null pointers to real GridCells. Any cell
 * that needs to be a child of this region is set correctly, while any cell
 * belonging to a child region has its pointer copied over. This ensures that
 * we don't have multiple copies of a GridCell floating around.
 *
 * Much of GridRegion's logic occurs in its helper functions:
 *      * allocateSpace(): makes enough room for the array based on the
 *              calculated width and height integers.
 *      * addBorders(): sets the cells of the outer ring of the region to be
 *              the proper border cells (including corners)
 *      * transferCells(): gives the current region's array to a child region,
 *              enabling it to copy over its own grid cell pointers into its
 *              parent region.
 *      * fillPadding(): fills up the rest of the space with padding cells -
 *              any cell not containing details will become empty space. This
 *              empty space cell basically ensures that every cell in the array
 *              is non-null, and that clicking this empty space cell actually
 *              refers to the proper grid region.
 */
class GridRegion
{
public:
    /* Constructor */
    GridRegion(TreeNode* n) : associatedNode(n) {}

    /* Make new nodes */
    GridRegion* addChildRegion(TreeNode* n);

    /* Where the magic happens */
    void buildGrid();

    /* Getters */
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
