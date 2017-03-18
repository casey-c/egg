#include "polishinputwidget.h"
#include "ui_polishinputwidget.h"

/*
 * Constructor
 */
PolishInputWidget::PolishInputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PolishInputWidget),
    polishRoot(nullptr),
    eggRoot(nullptr),
    wff(false),
    closeEarly(true)
{
    ui->setupUi(this);

    // Remove min/maximize buttons
    QWidget::setWindowFlags(Qt::Dialog);

    // Make sure we call the destructor on close()
    setAttribute(Qt::WA_DeleteOnClose);
}

/*
 * Destructor
 */
PolishInputWidget::~PolishInputWidget()
{
    delete ui;

    // Get rid of the allocated polish nodes
    if (polishRoot != nullptr)
        delete polishRoot;

    // Not handled in main window, so we'll delete any egg nodes here
    if (closeEarly && eggRoot != nullptr)
        delete eggRoot;
}

/*
 * Add function
 */
void PolishInputWidget::addNArityNode(int arity, QString t)
{
    // Determine if operator by checking the arity
    bool o = (arity > 0);

    // No root node yet
    if (polishRoot == nullptr)
    {
        polishRoot = PolishNode::makeRoot(t, o);
        eggRoot = new TreeNode();

        // Make the polish children
        for (int i = 0; i < arity; ++i)
            polishStack.push(polishRoot->makeNewChild());

        // Make the egg children
        if (QString::compare(t, "∧") == 0) // AND
        {
            eggStack.push(eggRoot);
            eggStack.push(eggRoot);
        }
        else if (QString::compare(t, "∨") == 0) // OR
        {
            TreeNode* outer = eggRoot->addChildCut();
            eggStack.push(outer->addChildCut());
            eggStack.push(outer->addChildCut());
        }
        else if (QString::compare(t, "¬") == 0) // NOT
        {
            eggStack.push(eggRoot->addChildCut());
        }
        else if (QString::compare(t, "→") == 0) // CONDITIONAL
        {
            TreeNode* outer = eggRoot->addChildCut();
            eggStack.push(outer->addChildCut());
            eggStack.push(outer);
        }
        else if (QString::compare(t, "↔") == 0) // BICONDITIONAL
        {
            TreeNode* outerRight = eggRoot->addChildCut();
            TreeNode* outerLeft = eggRoot->addChildCut();

            eggStack.push(outerRight);
            eggStack.push(outerRight->addChildCut());
            eggStack.push(outerLeft);
            eggStack.push(outerLeft->addChildCut());
        }
        else if (arity == 0) // Literals
        {
            eggRoot->addChildStatement(t);
        }

        // Redraw the plaintext
        redraw();

        // We've already have a WFF (i.e., added a literal only)
        if (polishStack.isEmpty())
        {
            wff = true;
            ui->add_button->setEnabled(true);
            ui->add_button->setFocus();
        }

        // Root creation won't do the rest
        return;
    }

    // We already have a WFF, so time to start a fresh one
    if (polishStack.isEmpty())
    {
        qDebug() << "Clearing root";
        ui->add_button->setEnabled(false);
        wff = false;

        // Delete the roots
        delete polishRoot;
        polishRoot = nullptr;
        delete eggRoot;
        eggRoot = nullptr;

        // Recall this function
        addNArityNode(arity, t);
        return;
    }

    // Pop, set, and add for polish
    PolishNode* poleNode = polishStack.pop();
    poleNode->setDetails(t, o);

    // Add the polish children
    for (int i = 0; i < arity; ++i)
        polishStack.push(poleNode->makeNewChild());

    // Pop, set, and add for egg
    TreeNode* eggNode = eggStack.pop();
    if (QString::compare(t, "∧") == 0) // AND
    {
        eggStack.push(eggNode);
        eggStack.push(eggNode);
    }
    else if (QString::compare(t, "∨") == 0) // OR
    {
        TreeNode* outer = eggNode->addChildCut();
        eggStack.push(outer->addChildCut());
        eggStack.push(outer->addChildCut());
    }
    else if (QString::compare(t, "¬") == 0) // NOT
    {
        eggStack.push(eggNode->addChildCut());
    }
    else if (QString::compare(t, "→") == 0) // CONDITIONAL
    {
        TreeNode* outer = eggNode->addChildCut();
        eggStack.push(outer->addChildCut());
        eggStack.push(outer);
    }
    else if (QString::compare(t, "↔") == 0) // BICONDITIONAL
    {
        TreeNode* outerRight = eggNode->addChildCut();
        TreeNode* outerLeft = eggNode->addChildCut();

        eggStack.push(outerRight);
        eggStack.push(outerRight->addChildCut());
        eggStack.push(outerLeft);
        eggStack.push(outerLeft->addChildCut());
    }
    else if (arity == 0) // Literals
    {
        eggNode->addChildStatement(t);
    }

    // Redraw the plaintext
    redraw();

    // Check if well-formed
    if (polishStack.isEmpty())
    {
        wff = true;
        ui->add_button->setEnabled(true);
        ui->add_button->setFocus();
    }
}

void PolishInputWidget::redraw()
{
    // Null check
    if (polishRoot == nullptr)
        ui->lineEdit->setText("___");
    else
        ui->lineEdit->setText(polishRoot->toPlaintext());
}

/*
 * Keybindings
 */
void PolishInputWidget::keyPressEvent(QKeyEvent *event)
{
    // Letters
    if (event->key() >= 65 && event->key() <= 90)
    {
        addNArityNode(0, event->text().toUpper());
        return;
    }

    // Operators
    switch (event->key())
    {
    case Qt::Key_Ampersand:
        addNArityNode(2, "∧");
        return;
    case Qt::Key_Bar:
        addNArityNode(2, "∨");
        return;
    case Qt::Key_AsciiTilde:
        addNArityNode(1, "¬");
        return;
    case Qt::Key_Dollar:
        addNArityNode(2, "→");
        return;
    case Qt::Key_Percent:
        addNArityNode(2, "↔");
        return;
    case Qt::Key_Escape:
        close();
        return;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
}

void PolishInputWidget::on_add_button_clicked()
{
    qDebug() << "Well formed!";
    closeEarly = false; // a little flag for our closeEvent() to not delete here

    // Send info to make the Tree update, also will clean up that memory
    emit(sendCompletedFormula(eggRoot));
    this->close();
}
