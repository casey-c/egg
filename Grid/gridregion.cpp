#include "gridregion.h"

void GridRegion::buildGrid()
{

}

GridRegion* GridRegion::addChildRegion(TreeNode *n)
{
    GridRegion* child = new GridRegion(n);
    children.append(child);

    return child;
}
