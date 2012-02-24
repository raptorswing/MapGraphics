#include "MapTileSource.h"

#include <QtDebug>
#include <QStringBuilder>
#include <QFileInfo>
#include <QDateTime>

const QString MAPGRAPHICS_CACHE_FOLDER_NAME = ".MapGraphicsCache";
const quint64 MAX_DISK_CACHE_READ_ATTEMPTS = 1000000;
const quint32 DEFAULT_MAX_DISK_CACHE_AGE = 7 * 24 * 60 * 60; //Seven days in seconds

MapTileSource::MapTileSource() :
    _maxDiskCacheTime(DEFAULT_MAX_DISK_CACHE_AGE)
{
    this->tempCacheLock = new QMutex();
    this->memoryCache.setMaxCost(20);

    /*
      We do this because MapTileSource runs in its own thread.
      It's a way to communicate from the GUI thread to the MapTileSource thread
    */
    connect(this,
            SIGNAL(tileRequested(quint16,quint16,quint16)),
            this,
            SLOT(fetchTile(quint16,quint16,quint16)));
}

MapTileSource::~MapTileSource()
{
    delete this->tempCacheLock;
}

void MapTileSource::requestTile(quint16 x, quint16 y, quint16 z)
{
    this->tileRequested(x,y,z);
}

QRectF MapTileSource::sceneRectFromGeoRect(const QRectF &latLonRect, quint8 zoomLevel) const
{
    QRectF toRet(this->scenePixelFromCoordinate(latLonRect.topLeft(),zoomLevel),
                 this->scenePixelFromCoordinate(latLonRect.bottomRight(),zoomLevel));
    return toRet;
}

QRectF MapTileSource::geoRectFromSceneRect(const QRectF &sceneRect, quint8 zoomLevel) const
{
    QRectF toRet(this->coordinateFromScenePixel(sceneRect.topLeft(),zoomLevel),
                 this->coordinateFromScenePixel(sceneRect.bottomRight(),zoomLevel));
    return toRet;
}

void MapTileSource::fetchTile(quint16 x, quint16 y, quint16 z)
{
    //Do Caching here
    if (this->isCachingOkay())
    {
        QImage * cached = this->fromMemCache(x,y,z);
        if (!cached)
        {
            cached = this->fromDiskCache(x,y,z);
        }

        if (cached)
        {
            this->notifyClientOfRetrieval(x,y,z,cached);
            return;
        }
    }

    //Pass it off to the child class's methods if the tile isn't cached
    this->asynchronousTileRequest(x,y,z);
}

QImage * MapTileSource::retrieveFinishedRequest(quint16 x, quint16 y, quint16 z)
{
    const QString cacheID = this->createCacheID(x,y,z);
    QMutexLocker lock(this->tempCacheLock);
    if (!this->tempCache.contains(cacheID))
    {
        qWarning() << "retrieveFinishedRequest called with unknown tile" << x << y << z;
        return 0;
    }
    return this->tempCache.take(cacheID);
}

qint32 MapTileSource::maxDiskCacheTime() const
{
    return this->_maxDiskCacheTime;
}

void MapTileSource::setMaxDiskCacheTime(qint32 nTime)
{
    this->_maxDiskCacheTime = nTime;
}

//protected
void MapTileSource::notifyClientOfRetrieval(quint16 x, quint16 y, quint16 z, QImage *tile)
{
    //If there's something wrong with the tile (corruption, etc) we throw it out.
    //This can happen sometimes with weird network things
    if (!this->isTileValid(tile))
    {
        delete tile;
        return;
    }

    this->insertIntoMemCache(x,y,z,tile);
    this->insertIntoDiskCache(x,y,z,tile);

    QMutexLocker lock(this->tempCacheLock);
    this->tempCache.insert(this->createCacheID(x,y,z),tile);
    this->tileRetrieved(x,y,z);
}

//protected
QImage * MapTileSource::fromMemCache(quint16 x, quint16 y, quint16 z)
{
    const QString cacheID = MapTileSource::createCacheID(x,y,z);
    QImage * toRet = 0;

    QImage * inCache = this->memoryCache.object(cacheID);
    if (inCache)
        toRet = new QImage(*inCache);

    return toRet;
}

//protected
QImage * MapTileSource::fromDiskCache(quint16 x, quint16 y, quint16 z)
{
    const QString path = this->getDiskCacheFile(x,y,z);
    QFile fp(path);
    if (!fp.exists())
        return 0;

    //If the cached tile is older than we would like, throw it out
    QFileInfo info(path);
    if (info.lastModified().secsTo(QDateTime::currentDateTime()) >= this->_maxDiskCacheTime)
    {
        if (!QFile::remove(path))
            qWarning() << "Failed to remove old cache file" << path;
        return 0;
    }

    if (!fp.open(QFile::ReadOnly))
    {
        qWarning() << "Failed to open" << QFileInfo(fp.fileName()).baseName() << "from cache";
        return 0;
    }

    QByteArray data;
    quint64 counter = 0;
    while (data.length() < fp.size())
    {
        data += fp.read(2048);
        if (++counter >= MAX_DISK_CACHE_READ_ATTEMPTS)
        {
            qWarning() << "Reading cache file" << fp.fileName() << "took too long. Aborting.";
            return 0;
        }
    }

    QImage * pixmap = new QImage();
    if (!pixmap->loadFromData(data))
    {
        delete pixmap;
        return 0;
    }

    return pixmap;
}

//protected static
QString MapTileSource::createCacheID(quint16 x, quint16 y, quint16 z)
{
    //More efficient QString concatenation using QStringBuilder
    QString toRet = QString::number(x) % "," % QString::number(y) % "," % QString::number(z);
    return toRet;
}

//protected static
bool MapTileSource::cacheID2xyz(const QString& string, quint16 *x, quint16 *y, quint16 *z)
{
    QStringList list = string.split(',');
    if (list.size() != 3)
    {
        qWarning() << "Bad cacheID" << string << "cannot convert";
        return false;
    }

    bool ok = true;
    *x = list.at(0).toUShort(&ok);
    if (!ok)
        return false;
    *y = list.at(1).toUShort(&ok);
    if (!ok)
        return false;
    *z = list.at(2).toUShort(&ok);
    return ok;
}

//private
QDir MapTileSource::getDiskCacheDirectory(quint16 x, quint16 y, quint16 z) const
{
    Q_UNUSED(y)
    QString pathString = QDir::homePath() % QDir::separator() % MAPGRAPHICS_CACHE_FOLDER_NAME % QDir::separator() % this->getNameOfSource() % QDir::separator() % QString::number(z) % QDir::separator() % QString::number(x);
    QDir toRet = QDir(pathString);
    if (!toRet.exists())
    {
        if (!toRet.mkpath(toRet.absolutePath()))
            qWarning() << "Failed to create cache directory" << toRet.absolutePath();
    }
    return toRet;
}

//private
QString MapTileSource::getDiskCacheFile(quint16 x, quint16 y, quint16 z) const
{
    QString toRet = this->getDiskCacheDirectory(x,y,z).absolutePath() % QDir::separator() % QString::number(y) % "." % this->getFileExtension();
    return toRet;
}

//private
void MapTileSource::insertIntoMemCache(quint16 x, quint16 y, quint16 z, QImage * pixmap)
{
    const QString cacheID = MapTileSource::createCacheID(x,y,z);

    //If we've already cached something, do not cache it again!
    if (this->memoryCache.contains(cacheID))
        return;

    QImage * copy = new QImage(*pixmap);
    this->memoryCache.insert(cacheID,
                             copy);
}

//private
void MapTileSource::insertIntoDiskCache(quint16 x, quint16 y, quint16 z, QImage *pixmap)
{
    const QString filePath = this->getDiskCacheFile(x,y,z);

    //If we've already cached something, do not cache it again
    QFile fp(filePath);
    if (fp.exists())
        return;

    //Auto-detect file format
    const char * format = 0;

    //No compression!
    const int quality = 100;

    if (!pixmap->save(filePath,format,quality))
        qWarning() << "Failed to put" << this->getNameOfSource() << x << y << z << "into disk cache";
}


