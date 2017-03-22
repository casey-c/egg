#ifndef GRID_H
#define GRID_H

#include "gridregion.h"
#include "Tree/treestate.h"

/*
 * The main Grid class. Grids are an intermediate representation of an
 * existential graph tree. They are comprised of GridRegions, which link the
 * original existential tree to a more visual depiction. GridRegions are trees
 * of their own: each GridRegion is a grid composed of GridCells, while certain
 * GridRegions may have certain areas of their grid marked as owned by other
 * GridRegion children.
 *
 * The overall Grid class is kind of lightweight outside of its constructor. The
 * constructor does a lot of initial work creating the Regions it needs from a
 * corresponding tree state, and putting it into a bottom-up order. This
 * ordering is necessary to build the GridRegions correctly: as a node in the
 * GridRegion tree is dependent on all of its children, the leaves must be
 * constructed first and then the tree can work its way up.
 *
 * Once the regions are made and ordered, they allocate space and make the
 * needed GridCells inside the GridRegion class.
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
