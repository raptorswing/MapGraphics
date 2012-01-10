#include "MapGraphicsItem.h"

#include "guts/MapInfoManager.h"
#include "guts/MapGraphicsScene.h"
#include "guts/PrivateQGraphicsItem.h"

#include <QtDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <cmath>


MapGraphicsItem::MapGraphicsItem(MapGraphicsItem * parent) :
    myScene(0), boolUseStaticSize(false)
{
    this->graphicsItem = new PrivateQGraphicsItem(this);
    this->graphicsItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsScenePositionChanges);

    this->setParentItem(parent);

}

MapGraphicsItem::~MapGraphicsItem()
{
    /*
      Delete all of our children before we delete our underyling QGraphicsItem which would cause them
      to delete theirs, which would leave MapGraphicsItems with already-deleted QGraphicsItems beneath them
      which would be bad.
    */
    QMutableListIterator<MapGraphicsItem *> iter(this->children);
    while (iter.hasNext())
    {
        MapGraphicsItem * next = iter.next();
        iter.remove();
        delete next;
    }

    //Delete our underyling QGraphicsItem
    delete this->graphicsItem;
}

QGraphicsItem * MapGraphicsItem::qGraphicsItem() const
{
    return this->graphicsItem;
}

void MapGraphicsItem::setPos(const QPointF &globalPos)
{
    if (this->myScene == 0)
    {
        qWarning() << "MapGraphicsItem" << this << "doesn't have a scene set";
        return;
    }
    const quint8 zoomLevel = this->myScene->getZoomLevel();
    this->globalPos = globalPos;

    QSharedPointer<MapTileSource> mapTileSource = MapInfoManager::getInstance()->getMapTileSource();
    QPointF scenePos = mapTileSource->scenePixelFromCoordinate(globalPos,zoomLevel);

    this->qGraphicsItem()->setPos(scenePos);
    /*
      itemChange is called as a result of QGraphicsItem::SetPos, even though we don't need it here.
      As a result, the code does a scene->geo conversion and sets globalPos again as if we were dragging
      with the mouse. This causes zooming far in/out to make items drift. The following line corrects this.
    */
    this->globalPos = globalPos;

    //This needs to be generalized to different map tile sources with possible different tiles sizes
    if (!this->useStaticSize())
        this->qGraphicsItem()->setScale(pow(2.0,zoomLevel));
}

QPointF MapGraphicsItem::pos() const
{
    return this->globalPos;
}

qreal MapGraphicsItem::longitude() const
{
    return this->globalPos.x();
}

qreal MapGraphicsItem::latitude() const
{
    return this->globalPos.y();
}

qreal MapGraphicsItem::zValue() const
{
    return this->qGraphicsItem()->zValue();
}

void MapGraphicsItem::setScene(MapGraphicsScene *scene)
{
    this->myScene = scene;
}

bool MapGraphicsItem::useStaticSize() const
{
    return this->boolUseStaticSize;
}

void MapGraphicsItem::setUseStaticSize(bool n)
{
    this->boolUseStaticSize = n;
}

qreal MapGraphicsItem::rotation() const
{
    return this->qGraphicsItem()->rotation();
}

void MapGraphicsItem::setRotation(qreal rotation)
{
    this->qGraphicsItem()->setRotation(rotation);
}

bool MapGraphicsItem::isSelected() const
{
    return this->qGraphicsItem()->isSelected();
}

void MapGraphicsItem::setSelected(bool nSelected)
{
    qDebug() << "Set selected" << nSelected;
    this->qGraphicsItem()->setSelected(nSelected);
}

void MapGraphicsItem::setParentItem(MapGraphicsItem *item)
{
    this->parent = item;
    if (item != 0)
    {
        item->addChild(this);
        item->scene()->addItem(this);
        this->qGraphicsItem()->setParentItem(item->qGraphicsItem());
        this->setScene(item->scene());
    }
}

//pure-virtual from PrivateQGraphicsItemParent
void MapGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
    qDebug() << "Context menu event" << event->pos() << event->scenePos() << event->screenPos();
}

//pure-virtual from PrivateQGraphicsItemParent
void MapGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDebug() << "Default item mouse press event" << event->pos() << event->scenePos() << event->screenPos();
}

//pure-virtual from PrivateQGraphicsItemParent
void MapGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Default item mouse release event" << event->pos() << event->scenePos() << event->screenPos();
}

//pure-virtual from PrivateQGraphicsItemParent
MapGraphicsScene * MapGraphicsItem::scene() const
{
    return this->myScene;
}

//pure-virtual from PrivateQGraphicsItemParent
QVariant MapGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemScenePositionHasChanged)
    {
        QPointF graphicsScenePixel = value.toPointF();
        QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
        if (!tileSource.isNull())
        {
            this->globalPos = tileSource->coordinateFromScenePixel(graphicsScenePixel,this->scene()->getZoomLevel());
            qDebug() << "Changed:" << this->globalPos;
        }
    }
    return value;
}

QPointF MapGraphicsItem::mapFromScene(const QPointF &point) const
{
    //We take a point in geographic coordinates. Need to map it to regular scene coordinates
    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
    QPoint scenePixel = tileSource->scenePixelFromCoordinate(point,this->scene()->getZoomLevel());

    //Now we need to map that to our item-space coordinates
    QPointF itemPos = this->graphicsItem->mapFromScene(scenePixel);

    return itemPos;
}

void MapGraphicsItem::update()
{
    this->graphicsItem->update();
}

//private
void MapGraphicsItem::addChild(MapGraphicsItem *child)
{
    if (!this->children.contains(child))
        this->children.append(child);
}
