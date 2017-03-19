#ifndef GRID_H
#define GRID_H

#include "gridregion.h"
#include "Tree/treestate.h"

/*
 * The main Grid class. Grids are made up of regions
 */
class Grid
{
public:
    /* Constructor will do all the neccessary set up of the grid */
    Grid(TreeState* t);

    /* Print */
    QString toPlaintext();

private:
    GridRegion* root;
    TreeState* tree;
};

#endif // GRID_H
