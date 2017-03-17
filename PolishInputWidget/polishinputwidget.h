#ifndef POLISHINPUTWIDGET_H
#define POLISHINPUTWIDGET_H

#include <QWidget>

namespace Ui {
class PolishInputWidget;
}

class PolishInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PolishInputWidget(QWidget *parent = 0);
    ~PolishInputWidget();

private:
    Ui::PolishInputWidget *ui;
};

#endif // POLISHINPUTWIDGET_H
