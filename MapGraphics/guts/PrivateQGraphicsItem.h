#ifndef PRIVATEQGRAPHICSITEM_H
#define PRIVATEQGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QSharedPointer>

#include "PrivateQGraphicsItemParent.h"

class PrivateQGraphicsItem : public QGraphicsItem
{
public:
    PrivateQGraphicsItem(PrivateQGraphicsItemParent * user);

    //pure-virtual from QGraphicsItem
    virtual QRectF boundingRect() const;

    //pure virtual from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //virtual from QGraphicsItem
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    //virtual from QGraphicsItem
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //virtual from QGraphicsItem
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);



private:
    PrivateQGraphicsItemParent * user;
};

#endif // PRIVATEQGRAPHICSITEM_H
