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
        int pos = 0;
        QChar c = line.at(pos);
        int x = c.digitValue();

        // NaN - so line isn't well formed
        if (x < 0)
        {
            qDebug() << "First char is not a number";
            continue;
        }

        qDebug() << "First char is a number and it is " << x;

        TreeState* state = new TreeState();
        TreeNode* root = state->getRoot();

        QStack<TreeNode*> stack;

        for (int i = 0; i < x; ++i)
            stack.push(root->addChildPlaceholder());

        while (!stack.isEmpty())
        {
            TreeNode* nextNode = stack.pop();

            if (line.length() < (pos + 1))
            {
                qDebug() << "Still stuff in stack, but we've run out of line!";
                qDebug() << "This indicates an invalid line";
                continue;
            }


            QChar nextChar = line.at(++pos);

            // Statement
            if (nextChar.digitValue() == -1)
            {
                qDebug() << "We have a statement of " << nextChar;
                QString s = "";
                s += nextChar;

                nextNode->addChildStatement(s);
            }
            else
            {
                qDebug() << "We have a cut of size " << nextChar.digitValue();
                TreeNode* cut = nextNode->addChildCut();

                // Push the number of children
                for (int i = 0; i < nextChar.digitValue(); ++i)
                    stack.push(cut->addChildPlaceholder());
            }

            qDebug() << "Finished an iteration, stack is size " << stack.size();

        }

        qDebug() << "Stack is empty";
        qDebug() << "Line length is " << line.length();
        qDebug() << "pos is " << pos;

        // Inside the while loop for now; later on we'll support multi-line
        // file input for actual steps in the proof
        //
        // At the moment, we only consider loading in the initial premise graph.

        file.close();

        MainWindow* mw = new MainWindow();
        mw->setCurrStateFromLoaded(state);
        return mw;
    }

    // All done
    //file.close();

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
