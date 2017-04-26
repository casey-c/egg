#include "goalitem.h"
#include "clickableframe.h"
#include "ui_clickableframe.h"
#include "hspacer.h"
#include "stepoverview.h"

#include <QLabel>
#include <QDebug>

GoalItem::GoalItem( QWidget* parent ) :
    ClickableFrame( parent ),
    isGood( false )
{
    ui->contentsLayout->addSpacerItem( HSpacer::makeSpacer() );
    ui->contentsLayout->addWidget( new QLabel("Goal") );

    xIcon = QPixmap( ":/Resources/goalx.svg" );
    xIcon = xIcon.scaled( QSize( 25, 25 ),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation );

    checkIcon = QPixmap( ":/Resources/check.svg" );
    checkIcon = checkIcon.scaled( QSize( 25, 25),
                                  Qt::KeepAspectRatio,
                                  Qt::SmoothTransformation );

    iconLabel = new QLabel();
    ui->contentsLayout->addWidget( iconLabel );
    updateIcon();

    ui->contentsLayout->addSpacerItem( HSpacer::makeSpacer() );
}

void GoalItem::updateIcon()
{
    if (isGood)
        iconLabel->setPixmap(checkIcon);
    else
        iconLabel->setPixmap(xIcon);
}

void GoalItem::onSuccessfulClick()
{
    ClickableFrame::onSuccessfulClick();

    qDebug() << "Clicked on goal";

    isGood = !isGood;
    updateIcon();
}
