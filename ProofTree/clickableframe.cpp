#include "clickableframe.h"
#include "ui_clickableframe.h"
#include "stepoverview.h"

#include <QDebug>
#include <QMouseEvent>

////////////////////////////////
/// Constructor / Destructor ///
////////////////////////////////

ClickableFrame::ClickableFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClickableFrame),
    overViewWidget(parent),
    isSelected(false)
{
    ui->setupUi(this);

    // Set-up the palette info. For now this is all hardcoded (poorly)
    //
    // TODO:
    // better design to allow all clickable frames to share the same palette
    // objects, so we don't have duplicates floating around
    //ui->frame->setAutoFillBackground(true);
    onDef = offDef = onSelect = offSelect = ui->frame->palette();

    onDef.setColor( backgroundRole(), QColor(140, 134, 160, 160) );
    offDef.setColor( backgroundRole(), QColor(0, 0, 0, 0) );
    onSelect.setColor( backgroundRole(), QColor(140, 134, 160, 255) );
    offSelect.setColor( backgroundRole(), QColor(140, 134, 160, 100) );

    // Auto select the newest created widget
    ((StepOverview*)overViewWidget)->selectChild(this);
}

ClickableFrame::~ClickableFrame()
{
    delete ui;
}

/////////////////
/// Selection ///
/////////////////

/*
 * Received an update from the parent to select this node
 */
void ClickableFrame::select()
{
    isSelected = true;
    updatePalette();
}

/*
 * Received an update from the parent to deselect this node
 */
void ClickableFrame::deselect()
{
    isSelected = false;
    updatePalette();
}

///////////////
/// Palette ///
///////////////

/*
 * Applies the proper palette based on mouse position and selection
 */
void ClickableFrame::updatePalette()
{
    if (underMouse())
    {
        if (isSelected)
            ui->frame->setPalette(onSelect);
        else
            ui->frame->setPalette(onDef);
    }
    else // Mouse not overtop
    {
        if (isSelected)
            ui->frame->setPalette(offSelect);
        else
            ui->frame->setPalette(offDef);
    }
}

//////////////////////
/// Mouse handling ///
//////////////////////

/*
 * Mouse down
 */
void ClickableFrame::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    ui->frame->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    updatePalette();
}

/*
 * Mouse released
 */
void ClickableFrame::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mouseReleaseEvent(event);
    ui->frame->setFrameStyle(QFrame::Panel | QFrame::Raised);

    // If the mouse is still on this widget, count this as a click
    if (this->rect().contains(event->pos()))
        onSuccessfulClick();

    updatePalette();
}

/*
 * Additional actions to perform on click. This should be extended by inheriting
 * classes... (make sure to call ClickableFrame::onSuccessfulClick() )
 */
//void ClickableFrame::onSuccessfulClick()
//{
    //((StepOverview*)overViewWidget)->selectChild(this);
//}

/*
 * Mouse enters the widget
 */
void ClickableFrame::enterEvent(QEvent *event)
{
    QWidget::enterEvent(event);
    updatePalette();
}

/*
 * Mouse leaves the widget
 */
void ClickableFrame::leaveEvent(QEvent *event)
{
    QWidget::leaveEvent(event);
    updatePalette();
}
