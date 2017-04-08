#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include <QUrl>
#include "MainWindow/mainwindow.h"

/*
 * FileConverter is a utility to convert between saved *.egg files and
 * application state. In essence, an *.egg file stores the actual steps and
 * proofs that are created with this application. These can be saved/loaded at
 * will.
 *
 * This class just converts an egg file into a ProofTree, making all the
 * necessary TreeStates that it reads from the file.
 *
 * At the present time, FileConverter only parses for valid input lines; no
 * checking is done to see if any of the steps logically follow from each other,
 * nor if the resulting application state even makes sense.
 *
 * Adding that functionality (making sure the *.egg file is properly formed) is
 * a major TODO item and needs to be completed before release.
 */
class FileConverter
{
public:
    /* Load from file */
    static MainWindow* load(QUrl filename);

    /* Save to file */
    static bool save(MainWindow* state);

private:
    /* Private constructor */
    FileConverter() {}
};

#endif // FILECONVERTER_H
