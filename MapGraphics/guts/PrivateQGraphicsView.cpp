#include "PrivateQGraphicsView.h"

#include <QWheelEvent>

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

void PrivateQGraphicsView::wheelEvent(QWheelEvent *event)
{
    //QGraphicsView::wheelEvent(event);
    this->hadWheelEvent(event);
}
