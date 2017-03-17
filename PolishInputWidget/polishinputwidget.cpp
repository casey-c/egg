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
    wff(false)
{
    ui->setupUi(this);
    QWidget::setWindowFlags(Qt::Dialog);
}

/*
 * Destructor
 */
PolishInputWidget::~PolishInputWidget()
{
    delete ui;
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

        for (int i = 0; i < arity; ++i)
            stack.push(polishRoot->makeNewChild());

        // Redraw the plaintext
        redraw();

        // We already have a WFF
        if (stack.isEmpty())
        {
            wff = true;
            ui->add_button->setEnabled(true);
            ui->add_button->setFocus();
        }

        // Root creation won't do the rest
        return;
    }

    // We already have a WFF
    if (stack.isEmpty())
    {
        qDebug() << "Clearing root";
        ui->add_button->setEnabled(false);
        wff = false;

        // Delete the root
        delete polishRoot;
        polishRoot = nullptr;

        // Recall this function
        addNArityNode(arity, t);
        return;
    }

    // Pop, set, and add
    PolishNode* node = stack.pop();
    node->setDetails(t, o);

    for (int i = 0; i < arity; ++i)
        stack.push(node->makeNewChild());

    // Redraw the plaintext
    redraw();

    // Check if well-formed
    if (stack.isEmpty())
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
        break;
    case Qt::Key_Bar:
        addNArityNode(2, "∨");
        break;
    case Qt::Key_AsciiTilde:
        addNArityNode(1, "¬");
        break;
    case Qt::Key_Dollar:
        addNArityNode(2, "→");
        break;
    case Qt::Key_Percent:
        addNArityNode(2, "↔");
        break;
    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void PolishInputWidget::on_add_button_clicked()
{
    if (wff)
        qDebug() << "Well formed!";
    else
        qDebug() << "Not well formed!";
}
