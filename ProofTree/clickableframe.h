#ifndef CLICKABLEFRAME_H
#define CLICKABLEFRAME_H

#include <QWidget>

namespace Ui {
class ClickableFrame;
}

class ClickableFrame : public QWidget
{
    Q_OBJECT

public:
    explicit ClickableFrame(QWidget *parent = 0);
    virtual ~ClickableFrame();

    /* Selection */
    void select();
    void deselect();

    /* Type identifier */
    virtual bool isGoal() { return false; }
    virtual bool isStep() { return false; }

protected:
    Ui::ClickableFrame *ui;
    QWidget* overViewWidget;

    bool isSelected;

    /* Palette stuff */
    QPalette onDef, offDef, onSelect, offSelect;
    void updatePalette();

    /* Extra actions performed by a valid click on this widget */
    virtual void onSuccessfulClick() = 0;

    /* Mouse handling */
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);

};

#endif // CLICKABLEFRAME_H
