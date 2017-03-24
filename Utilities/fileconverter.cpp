#include "fileconverter.h"

/*
 * Attempts to parse an *.egg input file and produce a new MainWindow with the
 * results.
 *
 * Params:
 *      filename: the URL identifying the target file
 *
 * Returns:
 *      * a MainWindow with the proper state, if valid file contents
 *      * nullptr if anything goes wrong
 */
MainWindow* FileConverter::load(QUrl filename)
{
    // Check if file exists
    if (!filename.isValid())
        return nullptr;

    // Attempt to create a tree state from the file
    // TODO: implementation

    return nullptr;
}


/*
 * Attempts to convert the passed mainwindow state into a valid *.egg file.
 *
 * Params:
 *      state: a pointer to the window that needs saving
 *
 * Returns:
 *      true, if successfully saved to disk
 *      false if anything goes wrong
 */
bool FileConverter::save(MainWindow *state)
{
    // Paranoid null check
    if (state == nullptr)
        return false;

    // TODO: implementation

    return false;
}
