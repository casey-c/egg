#ifndef GRIDCELL_H
#define GRIDCELL_H

#include "gridregion.h"
#include <QString>

/*
 * An enum to specify the CellType. This will be useful later on for drawing
 * the GridCells on a canvas.
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

// Forward declaration for GridRegion
class GridRegion;

/*
 * A GridCell is the data for a specific cell in the grid. It stores the
 * associated GridRegion (i.e. which region it is a direct child of) as well
 * as an associated type (defined by the CellType enum).
 *
 * cellLetter type will also store a QString for which letter it depicts.
 *
 * All the types store a plaintext string to be printed to the screen on a
 * toPlaintext() call of the parent region. These exist to make printing easy,
 * but in the future grid cells will be drawn in a 2D graphical way to some
 * canvas based on their type itself.
 *
 * GridCell uses the named constructor idiom: this makes it easier and more
 * intuitive to construct a cell based on what type of cell is desired. This
 * allows cleaner, more understandable code inside the GridRegion class, as well
 * as not needing to use the enum outside of here.
 */
class GridCell
{
public:
    /* Named constructors */
    static GridCell* makeLetter(GridRegion* p, QString l) {
        return new GridCell(p, cellLetter, l);
    }
    static GridCell* makePadding(GridRegion* p) {
        return new GridCell(p, cellPadding, " ");
    }
    static GridCell* makeCornerUpperLeft(GridRegion* p) {
        return new GridCell(p, cellBorderCornerUpperLeft, "┌");
    }
    static GridCell* makeCornerUpperRight(GridRegion* p) {
        return new GridCell(p, cellBorderCornerUpperRight, "┐");
    }
    static GridCell* makeCornerLowerLeft(GridRegion* p) {
        return new GridCell(p, cellBorderCornerLowerLeft, "└");
    }
    static GridCell* makeCornerLowerRight(GridRegion* p) {
        return new GridCell(p, cellBorderCornerLowerRight, "┘");
    }
    static GridCell* makeBorderHorizontal(GridRegion* p) {
        return new GridCell(p, cellBorderHorizontal, "─");
    }
    static GridCell* makeBorderVertical(GridRegion* p) {
        return new GridCell(p, cellBorderVertical, "│");
    }

    /* Getters */
    QString getText() { return letter; }
    CellType getType() { return type; }

private:
    /* Private constructor to force using the static factories */
    GridCell(GridRegion* p, CellType c, QString l) :
        parent(p),
        type(c),
        letter(l) {}

    /* Details */
    GridRegion* parent;
    CellType type;

    QString letter;
};

#endif // GRIDCELL_H
