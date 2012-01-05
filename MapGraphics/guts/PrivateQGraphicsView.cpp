#include "PrivateQGraphicsView.h"

#include <QPainter>
#include <QTimer>
#include <QMouseEvent>
#include <QtDebug>
#include <QGraphicsItem>

PrivateQGraphicsView::PrivateQGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->setBackgroundBrush(QBrush(Qt::black));

    //We update the entire screen when we update
    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

//protected
void PrivateQGraphicsView::mousePressEvent(QMouseEvent *event)
{
    //Let MapGraphicsView have a turn
    this->hadMousePressEvent(event);

    if (event->isAccepted())
        return;

    //Let underlying QGraphicsView have a turn
    QGraphicsView::mousePressEvent(event);
}

//protected
void PrivateQGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    //Let MapGraphicsView have a turn
    this->hadMouseReleaseEvent(event);

    if (event->isAccepted())
        return;

    //Let underlying QGraphicsView have a turn
    QGraphicsView::mouseReleaseEvent(event);

    /*
    QGraphicsItem * item = this->itemAt(event->pos());

    //If we clicked nothing or a tile then we want to let someone get this event
    if (item == 0 || item->zValue() <= 0.0)
        event->ignore();
    //If we clicked a non-tile item then we want to consider the event accepted
    else
        event->accept();
        */
}

//protected
void PrivateQGraphicsView::resizeEvent(QResizeEvent *event)
{
    //Underlying stuff needs to know about resize events so that the map does the right thing
    QGraphicsView::resizeEvent(event);

    this->hadResizeEvent(event);
}

//protected
void PrivateQGraphicsView::wheelEvent(QWheelEvent *event)
{
    /*
        We don't pass on wheel events because we don't want the scrollbars to
        respond to them.
    */
    //QGraphicsView::wheelEvent(event);


    this->hadWheelEvent(event);
}



