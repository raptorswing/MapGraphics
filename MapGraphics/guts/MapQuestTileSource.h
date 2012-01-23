#ifndef MAPQUESTTILESOURCE_H
#define MAPQUESTTILESOURCE_H

#include "MapTileSource.h"
#include "MapGraphics_global.h"

#include <QNetworkReply>
#include <QHash>
#include <QPixmap>
#include <QSharedPointer>
#include <QCache>
#include <QSet>

class MAPGRAPHICSSHARED_EXPORT MapQuestTileSource : public MapTileSource
{
    Q_OBJECT
public:
    enum MapQuestTileType
    {
        OSM_TILES,
        AERIAL_TILES
    };

    explicit MapQuestTileSource(MapQuestTileType tileType = OSM_TILES);

    //from MapTileSource
    virtual QPoint scenePixelFromCoordinate(const QPointF& latlon,quint8 zoomLevel) const;

    //from MapTileSource
    virtual QPointF coordinateFromScenePixel(const QPointF& point,quint8 zoomLevel) const;

    //from MapTileSource
    virtual quint16 getTileSize() const;

    //from MapTileSource
    virtual quint8 getMinZoomLevel() const;

    //from MapTileSource
    virtual quint8 getMaxZoomLevel() const;

    //from MapTileSource
    virtual QString getNameOfSource() const;

    //from MapTileSource
    virtual QString getFileExtension() const;

protected:
    //from MapTileSource
    virtual bool asynchronousTileRequest(quint16 x,
                                         quint16 y,
                                         quint16 z);

    //from MapTileSource
    virtual bool isTileValid(const QImage *image) const;

    //from MapTileSource
    virtual bool isCachingOkay() const;

signals:

public slots:

private slots:
    void handleNetworkRequestFinished();

private:
    QHash<QNetworkReply *, QString> pendingReplies;
    QSet<QString> pendingRequests;

    const MapQuestTileType tileType;

    static qreal rad2deg(qreal);
    static qreal deg2rad(qreal);
};

#endif // MAPQUESTTILESOURCE_H
