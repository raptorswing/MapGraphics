#ifndef PRIVATEQGRAPHICSITEMPARENT_H
#define PRIVATEQGRAPHICSITEMPARENT_H

#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsItem>

class MapGraphicsScene;

class PrivateQGraphicsItemParent
{
public:
    PrivateQGraphicsItemParent();
    virtual ~PrivateQGraphicsItemParent();

    //QGraphicsItem calls this
    virtual QRectF boundingRect() const=0;

    //QGraphicsItem calls this
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)=0;

    //QGraphicsItem calls this
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *)=0;

    //QGraphicsItem calls this
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *)=0;

    //QGraphicsItem calls this
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *)=0;

    //PrivateQGraphicsItem calls this
    virtual MapGraphicsScene * scene() const=0;

    //PrivateQGraphicsItem calls this
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)=0;


};

#endif // PRIVATEQGRAPHICSITEMPARENT_H
