#include "CompositeTileSource.h"

#include <QtDebug>
#include <QPainter>

CompositeTileSource::CompositeTileSource(QObject *parent) :
    MapTileSource(parent)
{
    this->setCacheMode(MapTileSource::NoCaching);
}

CompositeTileSource::~CompositeTileSource()
{
    qDebug() << this << "destructing";
    //Clean up all data related to pending tiles
    QList<QMap<quint32, QImage *> * > pendingTiles = _pendingTiles.values();
    for (int i = 0; i < pendingTiles.size(); i++)
    {
        QMap<quint32, QImage *> * tiles = pendingTiles.at(i);
        foreach(QImage * tile, tiles->values())
            delete tile;
        delete tiles;
    }
    _pendingTiles.clear();

    //Clear the sources
    _childSources.clear();
}

QPointF CompositeTileSource::ll2qgs(const QPointF &ll, quint8 zoomLevel) const
{
    if (_childSources.isEmpty())
    {
        qWarning() << "Composite tile source is empty --- results undefined";
        return QPointF(0,0);
    }

    //Assume they're all the same. Nothing to do otherwise!
    return _childSources.at(0)->ll2qgs(ll,zoomLevel);
}

QPointF CompositeTileSource::qgs2ll(const QPointF &qgs, quint8 zoomLevel) const
{
    if (_childSources.isEmpty())
    {
        qWarning() << "Composite tile source is empty --- results undefined";
        return QPointF(0,0);
    }

    //Assume they're all the same. Nothing to do otherwise!
    return _childSources.at(0)->qgs2ll(qgs,zoomLevel);
}

quint64 CompositeTileSource::tilesOnZoomLevel(quint8 zoomLevel) const
{
    if (_childSources.isEmpty())
        return 1;
    else
        return _childSources.at(0)->tilesOnZoomLevel(zoomLevel);
}

quint16 CompositeTileSource::tileSize() const
{
    if (_childSources.isEmpty())
        return 256;
    else
        return _childSources.at(0)->tileSize();
}

quint8 CompositeTileSource::minZoomLevel(QPointF ll)
{
    //Return the highest minimum
    quint8 highest = 0;

    foreach(QSharedPointer<MapTileSource> source, _childSources)
    {
        quint8 current = source->minZoomLevel(ll);
        if (current > highest)
            highest = current;
    }
    return highest;
}

quint8 CompositeTileSource::maxZoomLevel(QPointF ll)
{
    //Return the lowest maximum
    quint8 lowest = 50;

    foreach(QSharedPointer<MapTileSource> source, _childSources)
    {
        quint8 current = source->maxZoomLevel(ll);
        if (current < lowest)
            lowest = current;
    }
    return lowest;
}

QString CompositeTileSource::name() const
{
    return "Composite Tile Source";
}

QString CompositeTileSource::tileFileExtension() const
{
    return ".jpg";
}

void CompositeTileSource::addSourceTop(QSharedPointer<MapTileSource> source, qreal opacity)
{
    if (source.isNull())
        return;

    _childSources.insert(0, source);
    _childOpacities.insert(0,opacity);

    connect(source.data(),
            SIGNAL(tileRetrieved(quint32,quint32,quint8)),
            this,
            SLOT(handleTileRetrieved(quint32,quint32,quint8)));
}

void CompositeTileSource::addSourceBottom(QSharedPointer<MapTileSource> source, qreal opacity)
{
    if (source.isNull())
        return;

    _childSources.append(source);
    _childOpacities.append(opacity);

    connect(source.data(),
            SIGNAL(tileRetrieved(quint32,quint32,quint8)),
            this,
            SLOT(handleTileRetrieved(quint32,quint32,quint8)));
}

//protected
void CompositeTileSource::fetchTile(quint32 x, quint32 y, quint8 z)
{
    //If we have no child sources, just print a message about that
    if (_childSources.isEmpty())
    {
        QImage * toRet = new QImage(this->tileSize(),
                                    this->tileSize(),
                                    QImage::Format_ARGB32_Premultiplied);
        QPainter painter(toRet);
        painter.drawText(toRet->rect(),
                         QString("Composite Source Empty"),
                         QTextOption(Qt::AlignCenter));
        painter.end();
        this->prepareRetrievedTile(x,y,z,toRet);
        return;
    }

    //Mark the request so we can build tiles as they come
    QString cacheID = MapTileSource::createCacheID(x,y,z);
    _pendingTiles.insert(cacheID,new QMap<quint32,QImage *>());

    //Request tiles from all of our beautiful children
    for (int i = 0; i < _childSources.size(); i++)
    {
        QSharedPointer<MapTileSource> child = _childSources.at(i);
        child->requestTile(x,y,z);
    }
}

//private slot
void CompositeTileSource::handleTileRetrieved(quint32 x, quint32 y, quint8 z)
{
    QObject * sender = QObject::sender();
    MapTileSource * tileSource = qobject_cast<MapTileSource *>(sender);

    //Make sure this slot was called from a signal off a MapTileSource
    if (!tileSource)
    {
        qWarning() << this << "failed MapTileSource cast";
        return;
    }

    //Make sure this is a notification from a MapTileSource that we care about
    int tileSourceIndex = -1;
    for (int i = 0; i < _childSources.size(); i++)
    {
        if (_childSources[i].data() != tileSource)
            continue;
        tileSourceIndex = i;
        break;
    }

    if (tileSourceIndex == -1)
    {
        qWarning() << this << "received tile from unknown source...";
        return;
    }


    //Make sure that this is a tile we're interested in
    const QString cacheID = MapTileSource::createCacheID(x,y,z);
    if (!_pendingTiles.contains(cacheID))
    {
        qWarning() << this << "received unknown tile" << x << y << z << "from" << tileSource;
        return;
    }

    //Make sure the tile is non-null
    QImage * tile = tileSource->getFinishedTile(x,y,z);
    if (!tile)
    {
        qWarning() << this << "received null tile" << x << y << z << "from" << tileSource;
        return;
    }

    //qDebug() << this << "Retrieved tile" << x << y << z << "from" << tileSource;

    /*
      Put the tile into our pendingTiles structure. If it was the last tile we wanted, build
      our finishied product and notify our client
    */
    QMap<quint32, QImage *> * tiles = _pendingTiles.value(cacheID);
    tiles->insert(tileSourceIndex,tile);
    if (tiles->size() < _childSources.size())
        return;

    QImage * toRet = new QImage(this->tileSize(),
                                this->tileSize(),
                                QImage::Format_ARGB32_Premultiplied);
    QPainter painter(toRet);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setOpacity(1.0);
    for (int i = tiles->size()-1; i >= 0; i--)
    {
        QImage * childTile = tiles->value(i);
        painter.setOpacity(_childOpacities.at(i));
        painter.drawImage(0,0,*childTile);
        delete childTile;
    }
    delete tiles;
    _pendingTiles.remove(cacheID);
    painter.end();

    this->prepareRetrievedTile(x,y,z,toRet);
}
