#include "fileconverter.h"
#include <QMessageBox>

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

    qDebug() << "Attempting to load from " << filename.toString();

    // Load the file
    QFile file(filename.toLocalFile());
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0,
                                 "ERROR: couldn't open file ",
                                 file.errorString());
    }

    // Parse the file line by line
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();

        // Blank line
        if (line.isEmpty())
            continue;

        qDebug() << "line: " << line;

        // Parse the line char by char
        QChar c = line.at(0);
        int x = c.digitValue();
        qDebug() << "First char is " << x;
    }

    // All done
    file.close();

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
