#include "MapQuestTileSource.h"

#include <QNetworkRequest>
#include <QtDebug>
#include <QTimer>
#include <cmath>

#include "MGNetworkManager.h"

#define PI 3.14159265358979323846

MapQuestTileSource::MapQuestTileSource(MapQuestTileType tileType) : tileType(tileType)
{
}

bool MapQuestTileSource::asynchronousTileRequest(quint16 x,
                                                 quint16 y,
                                                 quint16 z)
{
    MGNetworkManager * netman = MGNetworkManager::getInstance();

    QString host;
    QString url;
    if (this->tileType == OSM_TILES)
    {
        host = "http://otile1.mqcdn.com";
        url = "/tiles/1.0.0/osm/%1/%2/%3.jpg";
    }
    else
    {
        host = "http://oatile1.mqcdn.com";
        url = "/naip/%1/%2/%3.jpg";
    }

    const QString cacheID = this->createCacheID(x,y,z);

    /*
      We don't want to make multiple requests for the same imagetile at the same time.
    */
    if (this->pendingRequests.contains(cacheID))
        return true;
    this->pendingRequests.insert(cacheID);

    //Build the request
    const QString fetchURL = url.arg(QString::number(z),QString::number(x),QString::number(y));
    const QNetworkRequest request(QUrl(host + fetchURL));

    //Send the request and make sure we're notified when it's finished
    QNetworkReply * reply = netman->get(request);
    this->pendingReplies.insert(reply,cacheID);

    connect(reply,
            SIGNAL(finished()),
            this,
            SLOT(handleNetworkRequestFinished()));

    return true;
}

QPoint MapQuestTileSource::scenePixelFromCoordinate(const QPointF& latlon, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->getTileSize();
    qreal x = (latlon.x()+180) * (tilesOnOneEdge*tileSize)/360.; // coord to pixel!
    qreal y = (1-(log(tan(PI/4+deg2rad(latlon.y())/2)) /PI)) /2  * (tilesOnOneEdge*tileSize);

    return QPoint(int(x), int(y));
}

QPointF MapQuestTileSource::coordinateFromScenePixel(const QPointF& point, quint8 zoomLevel) const
{
    const qreal tilesOnOneEdge = pow(2.0,zoomLevel);
    const quint16 tileSize = this->getTileSize();
    qreal longitude = (point.x()*(360/(tilesOnOneEdge*tileSize)))-180;
    qreal latitude = rad2deg(atan(sinh((1-point.y()*(2/(tilesOnOneEdge*tileSize)))*PI)));

    return QPointF(longitude, latitude);
}

quint16 MapQuestTileSource::getTileSize() const
{
    return 256;
}

quint8 MapQuestTileSource::getMinZoomLevel() const
{
    return 0;
}

quint8 MapQuestTileSource::getMaxZoomLevel() const
{
    return 17;
}

QString MapQuestTileSource::getNameOfSource() const
{
    if (this->tileType == OSM_TILES)
        return "MapQuest OSM Tiles";
    else
        return "MapQuest Open Aerial Tiles";
}

QString MapQuestTileSource::getFileExtension() const
{
    return "jpg";
}

//protected
bool MapQuestTileSource::isTileValid(const QPixmap *pixmap) const
{
    if (pixmap == 0)
        return false;
    QImage image = pixmap->toImage();

    //If all colors are shades of gray (red, green, blue components equal), we assume bad image
    return !image.allGray();
}

//protected
bool MapQuestTileSource::isCachingOkay() const
{
    return true;
}

//private slot
void MapQuestTileSource::handleNetworkRequestFinished()
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

    if (!this->pendingReplies.contains(reply))
        return;

    //Get the cacheID associated with the reply
    const QString cacheID = this->pendingReplies.value(reply);
    this->pendingReplies.remove(reply);
    this->pendingRequests.remove(cacheID);

    //If the request had an error we don't want to do anything with it
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Network error:" << reply->errorString();
        return;
    }


    //Convert the cacheID back into x,y,z tile coordinates
    quint16 x,y,z;
    if (!this->cacheID2xyz(cacheID,&x,&y,&z))
        return;

    QByteArray bytes = reply->readAll();
    QPixmap * pixmap = new QPixmap();

    if (!pixmap->loadFromData(bytes))
    {
        delete pixmap;
        return;
    }
    //Add to the "fake" temporary cache that the MapTileGraphicsItems pull from
    this->notifyClientOfRetrieval(x,y,z,pixmap);
}

//private static
qreal MapQuestTileSource::rad2deg(qreal x)
{
    return x*(180.0/PI);
}

//private static
qreal MapQuestTileSource::deg2rad(qreal x)
{
    return x*(PI/180.0);
}
