#ifndef MAPGRAPHICSITEM_H
#define MAPGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPointer>

#include "guts/MapGraphics_global.h"
#include "guts/PrivateQGraphicsItemParent.h"
class MapGraphicsScene;

class MAPGRAPHICSSHARED_EXPORT MapGraphicsItem : private PrivateQGraphicsItemParent
{
public:
    //explicit MapGraphicsItem(QGraphicsItem * qGraphicsItem);
    MapGraphicsItem(MapGraphicsItem * parent = 0);
    virtual ~MapGraphicsItem();

    QGraphicsItem * qGraphicsItem() const;

    void setPos(const QPointF& globalPos);
    QPointF pos() const;

    qreal longitude() const;
    qreal latitude() const;
    qreal zValue() const;

    void setScene(MapGraphicsScene * scene);

    bool useStaticSize() const;
    void setUseStaticSize(bool);

    qreal rotation() const;
    void setRotation(qreal rotation);

    bool isSelected() const;
    void setSelected(bool nSelected);

    void setParentItem(MapGraphicsItem * item = 0);

    //pure-virtual from PrivateQGraphicsItemParent
    //virtual QRectF boundingRect() const;

    //pure-virtual from PrivateQGraphicsItemParent
    //virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //pure-virtual from PrivateQGraphicsItemParent
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *);

    //pure-virtual from PrivateQGraphicsItemParent
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *);

    //pure-virtual from PrivateQGraphicsItemParent
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

    //pure-virtual from PrivateQGraphicsItemParent
    virtual MapGraphicsScene * scene() const;

    //pure-virtual from PrivateQGraphicsItemParent
    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value);

private:
    void addChild(MapGraphicsItem * child);


    QPointF globalPos;

    bool boolUseStaticSize;

    QGraphicsItem * graphicsItem;
    MapGraphicsScene * myScene;

    MapGraphicsItem * parent;
    QList<MapGraphicsItem *> children;

};

#endif // MAPGRAPHICSITEM_H
