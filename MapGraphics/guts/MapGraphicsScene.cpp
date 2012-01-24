#include "MapGraphicsScene.h"

#include <cmath>
#include <QQueue>
#include <QtDebug>

MapGraphicsScene::MapGraphicsScene(QObject *parent) :
    QObject(parent), zoomLevel(255)
{
    this->graphicsScene = new QGraphicsScene(this);

    //This causes the initial drawing to happen on startup
    this->setZoomLevel(0);
}

MapGraphicsScene::~MapGraphicsScene()
{
}


QGraphicsScene * MapGraphicsScene::scene() const
{
    return this->graphicsScene;
}

quint8 MapGraphicsScene::getZoomLevel() const
{
    return this->zoomLevel;
}

void MapGraphicsScene::addItem(MapGraphicsItem *item)
{
    this->mapGraphicsItems.insert(item,item->qGraphicsItem());
    this->qGraphicsItems.insert(item->qGraphicsItem(),item);
    this->scene()->addItem(item->qGraphicsItem());
    item->setScene(this);
}

QList<MapGraphicsItem *> MapGraphicsScene::items() const
{
    QList<MapGraphicsItem *> toRet;

    QList<QGraphicsItem *> qItems = this->graphicsScene->items();
    foreach(QGraphicsItem * qItem, qItems)
    {
        /*
          If it's not in our qGraphicsItems hash, then it's a tile and we don't
          want to return tiles to the user!
        */
        if (!this->qGraphicsItems.contains(qItem))
            continue;

        //Add the MapGraphicsItem that wraps this QGraphicsItem to our return list
        toRet.append(this->qGraphicsItems.value(qItem));
    }

    return toRet;
}

//public slot
void MapGraphicsScene::setViewerPosition(QPointF centerPoint, QPolygonF viewport)
{
    //Here we'll move tiles around and tell them to display the right things.
    //This is kind of the guts of what makes the "slippy map" work

    //Find tiles that we aren't displaying and mark them as "free"
    QQueue<MapTileGraphicsItem *> freeTiles;
    QList<QGraphicsItem *> visibleItems = this->scene()->items(viewport);

    foreach(MapTileGraphicsItem * item, this->tileDisplays)
    {
        if (!item->isVisible() || !visibleItems.contains(item))
        {
            freeTiles.enqueue(item);
            item->setVisible(false);
        }
    }

    const QRectF boundingRect = viewport.boundingRect();
    //int perSide = floor(sqrt((double)this->displayTilesNeeded));
    qint32 perSide = qMax(boundingRect.width()/this->tileSize,
                       boundingRect.height()/this->tileSize) + 3;
    qint32 xc = qMax((qint32)0,
                     (qint32)(centerPoint.x() / this->tileSize) - perSide/2);
    qint32 yc = qMax((qint32)0,
                     (qint32)(centerPoint.y() / this->tileSize) - perSide/2);
    const qint32 xMax = qMin((qint32)this->tilesPerRow,
                              xc + perSide);
    const qint32 yMax = qMin(yc + perSide,
                              (qint32)this->tilesPerCol);

    for (qint32 x = xc; x < xMax; x++)
    {
        for (qint32 y = yc; y < yMax; y++)
        {
            const QPointF scenePos(x*this->tileSize + this->tileSize/2,
                                   y*this->tileSize + this->tileSize/2);


            bool tileIsThere = false;
            foreach(QGraphicsItem * item, this->scene()->items(scenePos))
            {
                if (item->zValue() == -1.0 && item->isVisible())
                {
                    tileIsThere = true;
                    break;
                }
            }
            if (tileIsThere)
                continue;

            //Just in case we're running low on free tiles, add one
            if (freeTiles.isEmpty())
            {
                MapTileGraphicsItem * item = new MapTileGraphicsItem(this->tileSize);
                this->tileDisplays.append(item);
                this->scene()->addItem(item);
                freeTiles.enqueue(item);
            }
            //Get the first free tile and make it do its thing
            MapTileGraphicsItem * item = freeTiles.dequeue();
            item->setPos(scenePos);
            item->setVisible(true);
            item->setTileRequestInfo(x,y,this->zoomLevel);
        }
    }

}

//public slot
void MapGraphicsScene::setZoomLevel(int nZoom)
{
    nZoom = qMax<int>(0,nZoom);

    if (nZoom == this->zoomLevel)
        return;
    this->zoomLevel = nZoom;

    //This is stuff that we should factor out into specific map settings classes
    this->tilesOnZoomLevel = pow(4.0,this->zoomLevel);
    this->tilesPerRow = sqrt((long double)this->tilesOnZoomLevel);
    this->tilesPerCol = this->tilesPerRow;
    this->tileSize = 256;

    //Also, it's possible for tiles to not be square, so we should generalize that
    //We could use projections besides mercator
    this->scene()->setSceneRect(0,0,
                                this->tilesPerRow*this->tileSize,
                                this->tilesPerCol*this->tileSize);

    foreach(MapTileGraphicsItem * item,this->tileDisplays)
    {
        item->setVisible(false);
        //item->setPos(-5000,-5000);
    }

    //Update the positions of all our items
    this->updateItemsForZoomChange();

    this->zoomLevelChanged(nZoom);
}

//private slot
void MapGraphicsScene::updateItemsForZoomChange()
{
    QHashIterator<QGraphicsItem *, MapGraphicsItem *> iter(this->qGraphicsItems);
    while (iter.hasNext())
    {
        iter.next();
        //QGraphicsItem * key = iter.key();
        MapGraphicsItem * value = iter.value();

        qDebug() << "In:" << value->pos();
        value->setPos(value->pos());
        qDebug() << "Out:" << value->pos();
    }
}

void MapGraphicsScene::clearSelection()
{
    this->graphicsScene->clearSelection();
}

QList<MapGraphicsItem *> MapGraphicsScene::selectedItems() const
{
    QList<MapGraphicsItem *> toRet;

    foreach(QGraphicsItem * graphicsItem, this->scene()->selectedItems())
    {
        if (this->qGraphicsItems.contains(graphicsItem))
            toRet.append(this->qGraphicsItems.value(graphicsItem));
    }

    return toRet;
}


