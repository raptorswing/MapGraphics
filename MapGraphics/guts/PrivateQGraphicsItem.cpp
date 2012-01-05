#include "PrivateQGraphicsItem.h"

#include "MapInfoManager.h"
#include "MapGraphicsScene.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>


PrivateQGraphicsItem::PrivateQGraphicsItem(PrivateQGraphicsItemParent * user) : user(user)
{
    this->setZValue(1.0);
}

QRectF PrivateQGraphicsItem::boundingRect() const
{
    return this->user->boundingRect();
}

void PrivateQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    this->user->paint(painter,option,widget);
    painter->restore();
    if (option->state & QStyle::State_Selected)
    {
        const qreal penWidth = 0; // cosmetic pen

        const QColor fgcolor = option->palette.windowText().color();
        const QColor bgcolor( // ensure good contrast against fgcolor
                              fgcolor.red()   > 127 ? 0 : 255,
                              fgcolor.green() > 127 ? 0 : 255,
                              fgcolor.blue()  > 127 ? 0 : 255);

        painter->setPen(QPen(bgcolor, penWidth, Qt::SolidLine));
        painter->setBrush(Qt::NoBrush);
        const qreal pad = 0.2;
        painter->drawRect(this->boundingRect().adjusted(-pad, -pad, pad, pad));

        painter->setPen(QPen(option->palette.windowText(), penWidth, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(this->boundingRect().adjusted(-pad, -pad, pad, pad));
    }
}

void PrivateQGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QSharedPointer<MapTileSource> source = MapInfoManager::getInstance()->getMapTileSource();
    if (source.isNull())
        return;
    QGraphicsItem::contextMenuEvent(event);

    /*
    Then we can mangle the coordinates to geographic ones, but we have to mangle it back
    when we're done because this event may be used again
    */
    event->setScenePos(source->coordinateFromScenePixel(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
    this->user->contextMenuEvent(event);
    event->setScenePos(source->scenePixelFromCoordinate(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
}


void PrivateQGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QSharedPointer<MapTileSource> source = MapInfoManager::getInstance()->getMapTileSource();
    if (source.isNull())
        return;

    //Send the event to the parent class before we mangle the coordinates
    QGraphicsItem::mousePressEvent(event);

    /*
    Then we can mangle the coordinates to geographic ones, but we have to mangle it back
    when we're done because this event may be used again
    */
    event->setScenePos(source->coordinateFromScenePixel(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
    this->user->mousePressEvent(event);
    event->setScenePos(source->scenePixelFromCoordinate(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
}



void PrivateQGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QSharedPointer<MapTileSource> source = MapInfoManager::getInstance()->getMapTileSource();
    if (source.isNull())
        return;

    //Send the event to the parent class before we mangle the coordinates
    QGraphicsItem::mouseReleaseEvent(event);

    /*
    Then we can mangle the coordinates to geographic ones, but we have to mangle it back
    when we're done because this event may be used again
    */
    event->setScenePos(source->coordinateFromScenePixel(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
    this->user->mouseReleaseEvent(event);
    event->setScenePos(source->scenePixelFromCoordinate(event->scenePos(),
                                                        this->user->scene()->getZoomLevel()));
}

//protected
QVariant PrivateQGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return this->user->itemChange(change,value);
}

