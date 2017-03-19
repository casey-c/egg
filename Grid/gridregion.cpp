#include "gridregion.h"
#include <QDebug>

/*
 * Allocates space on the heap for a 2D array of GridCell pointers. This must be
 * called after the width and height have already been set.
 */
void GridRegion::allocateSpace()
{
    array = new GridCell**[width];
    for (int row = 0; row < width; ++row)
        array[row] = new GridCell*[height];

    // Make every GridCell a nullPtr until filled in later
    for (int col = 0; col < width; ++col)
        for (int row = 0; row < height; ++row)
            array[col][row] = nullptr;
}

/*
 * Fills any nullptr GridCell with a new padding cell. This must be called only
 * after allocating space.
 */
void GridRegion::fillPadding()
{
    for (int col = 0; col < width; ++col)
        for (int row = 0; row < height; ++row)
            if (array[col][row] == nullptr)
                array[col][row] = GridCell::makePadding(this);
}

/*
 * Adds any borders and corners for cut regions
 */
void GridRegion::addBorders()
{
    // Corners
    array[0][0] = GridCell::makeCornerUpperLeft(this);
    array[width - 1][0] = GridCell::makeCornerUpperRight(this);
    array[0][height - 1] = GridCell::makeCornerLowerLeft(this);
    array[width - 1][height - 1] = GridCell::makeCornerLowerRight(this);

    // Vertical border
    for (int row = 1; row < height - 1; ++row)
    {
        array[0][row] = GridCell::makeBorderVertical(this);
        array[width - 1][row] = GridCell::makeBorderVertical(this);
    }

    // Horizontal border
    for (int col = 1; col < width - 1; ++col)
    {
        array[col][0] = GridCell::makeBorderHorizontal(this);
        array[col][height - 1] = GridCell::makeBorderHorizontal(this);
    }
}

void GridRegion::buildGrid()
{
    // Leaf nodes are workable by themselves
    if (children.isEmpty())
    {
        // Statement
        if (associatedNode->isStatement())
        {
            width = height = 1;
            allocateSpace();

            array[0][0] = GridCell::makeLetter(this, associatedNode->getName());
        }
        // Empty Cut
        else if (associatedNode->isCut())
        {
            width = 5;
            height = 3;

            allocateSpace();
            addBorders();
            fillPadding(); // This will put a padding cell in the very center
        }
        // Empty root
        else if (associatedNode->isRoot())
        {
            width = height = 1;
            allocateSpace();
            fillPadding();
        }

        // At this point, this region is complete
        return;
    }
    else // We have some children, so we must be a cut or a root
    {
        // First, let's figure out our dimensions
        int widthSum = 0;
        int heightMax = 0;

        for (GridRegion* child : children)
        {
            widthSum += child->width;
            heightMax = std::max(child->height, heightMax);
        }

        // Calculated as follows:
        //      + total width of all the children
        //      + 1 for every gap between border/child and child/child
        width = widthSum + children.size() + 1;

        // Calculated as follows:
        //      + largest child's height
        height = heightMax;

        // Cuts have a border (root nodes don't, that's why this is separate)
        if (associatedNode->isCut())
        {
            width += 2;
            height += 2;
        }

        // We have the width and height, so we can allocate space
        allocateSpace();

        // Add borders for cut
        if (associatedNode->isCut())
            addBorders();

        // Transfer any child grid into our own

        // Starting x
        int posX = 1;

        // Account for the border
        if (associatedNode->isCut())
            ++posX;

        for (int i = 0; i < children.size(); ++i)
        {
            int childHeight = children.at(i)->height;
            int childWidth = children.at(i)->width;

            qDebug() << "attempting to transfer cells";
            qDebug() << width << " my width";
            qDebug() << height << " my height";
            qDebug() << childHeight << " childHeight";
            qDebug() << childWidth << " childWidth";

            children.at(i)->transferCells(posX,
                                          (height - childHeight) / 2,
                                          array);
            // Shift right
            posX += (childWidth + 1);
        }

        // Fill the rest in with padding and we should be a-okay
        fillPadding();
    }

}

GridRegion* GridRegion::addChildRegion(TreeNode *n)
{
    GridRegion* child = new GridRegion(n);
    children.append(child);

    return child;
}

void GridRegion::transferCells(int sx, int sy, GridCell ***parentArray)
{
    qDebug() << "transfering cells ";
    qDebug() << width << " my width";
    qDebug() << height << " my height";

    qDebug() << sx << " my sx";
    qDebug() << sy << " my sy";


    for (int pcol = sx, col = 0; col < width; ++pcol, ++col)
        for (int prow = sy, row = 0; row < height; ++prow, ++row)
        {
            qDebug() << pcol << " my pcol";
            qDebug() << prow << " my prow";
            qDebug() << col << " my col";
            qDebug() << row << " my row";

            parentArray[pcol][prow] = array[col][row];
        }
}

QString GridRegion::toString()
{
    qDebug() << "Called my to string method.";
    qDebug() << width << " width";
    qDebug() << height << " height";

    qDebug() << "I have " << children.size() << " children";
    int cW = 0;
    for (GridRegion* c : children)
        cW += c->width;
    qDebug() << "My children's total width is " << cW;

    QString result = "";
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
            result += array[col][row]->getText();

        result += "\n";
    }

    return result;
}
