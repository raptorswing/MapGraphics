#include "OSMTileSource.h"

#include "guts/MapGraphicsNetwork.h"

#include <cmath>
#include <QPainter>
#include <QStringBuilder>
#include <QtDebug>
#include <QNetworkReply>

const qreal PI = 3.14159265358979323846;
const qreal deg2rad = PI / 180.0;
const qreal rad2deg = 180.0 / PI;

OSMTileSource::OSMTileSource(OSMTileType tileType) :
    MapTileSource(), _tileType(tileType)
{
    this->setCacheMode(MapTileSource::DiskAndMemCaching);
}

OSMTileSource::~OSMTileSource()
{
    qDebug() << this << this->name() << "Destructing";
}

QPointF OSMTileSource::ll2qgs(const QPointF &ll, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal x = (ll.x()+180) * (tilesOnOneEdge*tileSize)/360; // coord to pixel!
    qreal y = (1-(log(tan(PI/4+(ll.y()*deg2rad)/2)) /PI)) /2  * (tilesOnOneEdge*tileSize);

    return QPoint(int(x), int(y));
}

QPointF OSMTileSource::qgs2ll(const QPointF &qgs, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->tileSize();
    qreal longitude = (qgs.x()*(360/(tilesOnOneEdge*tileSize)))-180;
    qreal latitude = rad2deg*(atan(sinh((1-qgs.y()*(2/(tilesOnOneEdge*tileSize)))*PI)));

    return QPointF(longitude, latitude);
}

quint64 OSMTileSource::tilesOnZoomLevel(quint8 zoomLevel) const
{
    return pow(4.0,zoomLevel);
}

quint16 OSMTileSource::tileSize() const
{
    return 256;
}

quint8 OSMTileSource::minZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    return 0;
}

quint8 OSMTileSource::maxZoomLevel(QPointF ll)
{
    Q_UNUSED(ll)
    return 18;
}

QString OSMTileSource::name() const
{
    switch(_tileType)
    {
    case OSMTiles:
        return "OpenStreetMap Tiles";
        break;

    case MapQuestOSMTiles:
        return "MapQuestOSM Tiles";
        break;

    case MapQuestAerialTiles:
        return "MapQuest Aerial Tiles";
        break;

    default:
        return "Unknown Tiles";
        break;
    }
}

QString OSMTileSource::tileFileExtension() const
{
    if (_tileType == OSMTiles || _tileType == MapQuestOSMTiles)
        return "png";
    else
        return "jpg";
}

//protected
void OSMTileSource::fetchTile(quint32 x, quint32 y, quint8 z)
{
    MapGraphicsNetwork * network = MapGraphicsNetwork::getInstance();

    QString host;
    QString url;

    //Figure out which server to request from based on our desired tile type
    if (_tileType == OSMTiles)
    {
        host = "http://b.tile.openstreetmap.org";
        url = "/%1/%2/%3.png";
    }
    else if (_tileType == MapQuestOSMTiles)
    {
        host = "http://otile1.mqcdn.com";
        url = "/tiles/1.0.0/osm/%1/%2/%3.jpg";
    }
    else
    {
        host = "http://otile1.mqcdn.com";
        url = "/tiles/1.0.0/sat/%1/%2/%3.jpg";
    }


    //Use the unique cacheID to see if this tile has already been requested
    const QString cacheID = this->createCacheID(x,y,z);
    if (_pendingRequests.contains(cacheID))
        return;
    _pendingRequests.insert(cacheID);

    //Build the request
    const QString fetchURL = url.arg(QString::number(z),
                                     QString::number(x),
                                     QString::number(y));
    QNetworkRequest request(QUrl(host + fetchURL));

    //Send the request and setupd a signal to ensure we're notified when it finishes
    QNetworkReply * reply = network->get(request);
    _pendingReplies.insert(reply,cacheID);

    connect(reply,
            SIGNAL(finished()),
            this,
            SLOT(handleNetworkRequestFinished()));
}

//private slot
void OSMTileSource::handleNetworkRequestFinished()
{
    QObject * sender = QObject::sender();
    QNetworkReply * reply = qobject_cast<QNetworkReply *>(sender);

    if (reply == 0)
    {
        qWarning() << "QNetworkReply cast failure";
        return;
    }

    /*
      We can do this here and use reply later in the function because the reply
      won't be deleted until execution returns to the event loop.
    */
    reply->deleteLater();

    if (!_pendingReplies.contains(reply))
    {
        qWarning() << "Unknown QNetworkReply";
        return;
    }

    //get the cacheID
    const QString cacheID = _pendingReplies.take(reply);
    _pendingRequests.remove(cacheID);

    //If there was a network error, ignore the reply
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network Error:" << reply->errorString();
        return;
    }

    //Convert the cacheID back into x,y,z tile coordinates
    quint32 x,y,z;
    if (!MapTileSource::cacheID2xyz(cacheID,&x,&y,&z))
    {
        qWarning() << "Failed to convert cacheID" << cacheID << "back to xyz";
        return;
    }

    QByteArray bytes = reply->readAll();
    QImage * image = new QImage();

    if (!image->loadFromData(bytes))
    {
        delete image;
        qWarning() << "Failed to make QImage from network bytes";
        return;
    }

    //Notify client of tile retrieval
    this->prepareRetrievedTile(x,y,z,image);
}
