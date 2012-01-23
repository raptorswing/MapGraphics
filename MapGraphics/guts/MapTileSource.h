#ifndef MAPTILESOURCE_H
#define MAPTILESOURCE_H

#include <QObject>
#include <QPixmap>
#include <QImage>
#include <QCache>
#include <QDir>
#include <QMutex>

class MapTileSource : public QObject
{
    Q_OBJECT
public:
    explicit MapTileSource();
    virtual ~MapTileSource();

    void requestTile(quint16 x, quint16 y, quint16 z);

    virtual QPoint scenePixelFromCoordinate(const QPointF& latlon,quint8 zoomLevel) const=0;
    virtual QPointF coordinateFromScenePixel(const QPointF& point,quint8 zoomLevel) const=0;

    virtual quint16 getTileSize() const=0;

    virtual quint8 getMinZoomLevel() const=0;
    virtual quint8 getMaxZoomLevel() const=0;

    virtual QString getNameOfSource() const=0;

    virtual QString getFileExtension() const=0;

    QImage * retrieveFinishedRequest(quint16 x, quint16 y, quint16 z);

    //quint32 getNextRequestID();



protected:
    virtual bool asynchronousTileRequest(quint16 x,
                                         quint16 y,
                                         quint16 z)=0;

    void notifyClientOfRetrieval(quint16 x, quint16 y, quint16 z, QImage * tile);

    virtual bool isCachingOkay() const=0;

    virtual bool isTileValid(const QImage * pixmap) const=0;

    QImage * fromMemCache(quint16 x, quint16 y, quint16 z);
    QImage * fromDiskCache(quint16 x, quint16 y, quint16 z);

    static QString createCacheID(quint16 x, quint16 y, quint16 z);
    static bool cacheID2xyz(const QString &, quint16 * x, quint16 * y, quint16 * z);


protected slots:
    void fetchTile(quint16 x,
                     quint16 y,
                     quint16 z);


signals:
    void tileRequested(quint16 x, quint16 y, quint16 z);
    void tileRetrieved(quint16 x, quint16 y, quint16 z);

private:
    QDir getDiskCacheDirectory(quint16 x, quint16 y, quint16 z) const;
    QString getDiskCacheFile(quint16 x, quint16 y, quint16 z) const;
    void insertIntoMemCache(quint16 x, quint16 y, quint16 z, QImage * pixmap);
    void insertIntoDiskCache(quint16 x, quint16 y, quint16 z, QImage * pixmap);

    /*
      Maps cacheID to an image that a client is waiting for. Temporary in nature.
      Kept just long enough for the person who requested the tile to grab it
    */
    QCache<QString, QImage> tempCache;
    QMutex * tempCacheLock;

    //A persistent in-memory cache
    QCache<QString, QImage> memoryCache;

};

#endif // MAPTILESOURCE_H
