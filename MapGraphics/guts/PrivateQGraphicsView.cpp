#include "PrivateQGraphicsView.h"

#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QtDebug>

PrivateQGraphicsView::PrivateQGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

PrivateQGraphicsView::PrivateQGraphicsView(QGraphicsScene *scene, QWidget *parent) :
    QGraphicsView(scene,parent)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

PrivateQGraphicsView::~PrivateQGraphicsView()
{
}

//protected
////virtual from QGraphicsView
void PrivateQGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    QGraphicsView::contextMenuEvent(event);
    if (!event->isAccepted())
        this->hadContextMenuEvent(event);
}

//protected
//virtual from QGraphicsView
void PrivateQGraphicsView::wheelEvent(QWheelEvent *event)
{
    //QGraphicsView::wheelEvent(event);
    this->hadWheelEvent(event);
}
