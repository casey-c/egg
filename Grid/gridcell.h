#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "gridregion.h"
#include <QString>

/*
 * An enum for the cell type: may be useless, may be useful, who knows
 */
enum CellType
{
    cellLetter,
    cellPadding,
    cellBorderHorizontal,
    cellBorderVertical,
    cellBorderCornerUpperLeft,
    cellBorderCornerUpperRight,
    cellBorderCornerLowerLeft,
    cellBorderCornerLowerRight
};

class GridRegion;

/*
 * Named constructors idiom
 */
class GridCell
{
public:
    /* Named constructors */
    static GridCell makeLetter(GridRegion* p, QString l) {
        return GridCell(p, cellLetter, l);
    }
    static GridCell makePadding(GridRegion* p) {
        return GridCell(p, cellPadding, " ");
    }
    static GridCell makeCornerUpperLeft(GridRegion* p) {
        return GridCell(p, cellBorderCornerUpperLeft, "┌");
    }
    static GridCell makeCornerUpperRight(GridRegion* p) {
        return GridCell(p, cellBorderCornerUpperRight, "┐");
    }
    static GridCell makeCornerLowerLeft(GridRegion* p) {
        return GridCell(p, cellBorderCornerLowerLeft, "└");
    }
    static GridCell makeCornerLowerRight(GridRegion* p) {
        return GridCell(p, cellBorderCornerLowerRight, "┘");
    }
    static GridCell makeBorderHorizontal(GridRegion* p) {
        return GridCell(p, cellBorderHorizontal, "─");
    }
    static GridCell makeBorderVertical(GridRegion* p) {
        return GridCell(p, cellBorderVertical, "│");
    }

    QString getText() { return letter; }
    CellType getType() { return type; }

private:
    GridCell(GridRegion* p, CellType c, QString l) :
        parent(p),
        type(c),
        letter(l) {}

    GridRegion* parent;
    CellType type;

    QString letter;
};

#endif // GRIDCELL_H
