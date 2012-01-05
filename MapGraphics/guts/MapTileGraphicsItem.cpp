#include "MapTileGraphicsItem.h"

#include <QPainter>
#include <QtDebug>

#include "guts/MapInfoManager.h"

MapTileGraphicsItem::MapTileGraphicsItem(quint16 tileSize) :
    tileSize(tileSize), tile(0), havePendingRequest(false)
{
    //So that the first (0,0,0) request doesn't get thrown out as being a duplicate
    this->tileZoom = 250;
    this->setZValue(-1.0);

    this->setAcceptedMouseButtons(0);
}

MapTileGraphicsItem::~MapTileGraphicsItem()
{
    if (this->tile != 0)
        delete this->tile;
}

QRectF MapTileGraphicsItem::boundingRect() const
{
    return QRectF(-1*this->getTileSize()/2,
                  -1*this->getTileSize()/2,
                  this->getTileSize(),
                  this->getTileSize());
}

void MapTileGraphicsItem::paint(QPainter *painter,
                                const QStyleOptionGraphicsItem *,
                                QWidget *)
{
    if (this->tile != 0)
        painter->drawPixmap(this->boundingRect().toRect(),*this->tile);
    else
        painter->fillRect(this->boundingRect(),Qt::gray);
}


quint16 MapTileGraphicsItem::getTileSize() const
{
    return this->tileSize;
}


void MapTileGraphicsItem::setTileRequestInfo(quint16 x, quint16 y, quint16 z)
{
    //Don't re-request the same tile we're alread displaying
    if (this->tileX == x && this->tileY == y && this->tileZoom == z)
        return;

    //Get rid of the old tile
    if (this->tile != 0)
    {
        delete this->tile;
        this->tile = 0;
    }


    this->tileX = x;
    this->tileY = y;
    this->tileZoom = z;

    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
    connect(tileSource.data(),
            SIGNAL(tileRetrieved(quint16,quint16,quint16)),
            this,
            SLOT(handleTileRetrieved(quint16,quint16,quint16)));
    this->havePendingRequest = true;
    tileSource->requestTile(x,y,z);
}

void MapTileGraphicsItem::handleTileRetrieved(quint16 x, quint16 y, quint16 z)
{
    if (!this->havePendingRequest)
        return;
    else if (this->tileX != x || this->tileY != y || this->tileZoom != z)
        return;
    this->havePendingRequest = false;

    QSharedPointer<MapTileSource>  tileSource = MapInfoManager::getInstance()->getMapTileSource();

    QPixmap * tile = tileSource->retrieveFinishedRequest(x,y,z);
    if (this->tile != 0)
        qDebug() << "Should be null";
    this->tile = tile;
    this->update();

    /*
      We disconnect so that a tile that isn't awaiting an update doesn't have to filter
      through a bunch of updates. Whether connectin and disconnecting repeatedly is
      cheaper than just sorting through extraneous updates remains to be seen.
      */
    QObject::disconnect(tileSource.data(),
                        SIGNAL(tileRetrieved(quint16,quint16,quint16)),
                        this,
                        SLOT(handleTileRetrieved(quint16,quint16,quint16)));
}
