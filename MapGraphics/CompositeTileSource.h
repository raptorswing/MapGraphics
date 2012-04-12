#ifndef COMPOSITETILESOURCE_H
#define COMPOSITETILESOURCE_H

#include "MapTileSource.h"
#include "MapGraphics_global.h"

#include <QList>
#include <QHash>
#include <QMap>
#include <QSharedPointer>

class MAPGRAPHICSSHARED_EXPORT CompositeTileSource : public MapTileSource
{
    Q_OBJECT
public:
    explicit CompositeTileSource(QObject *parent = 0);
    virtual ~CompositeTileSource();

    virtual QPointF ll2qgs(const QPointF& ll, quint8 zoomLevel) const;

    virtual QPointF qgs2ll(const QPointF& qgs, quint8 zoomLevel) const;

    virtual quint64 tilesOnZoomLevel(quint8 zoomLevel) const;

    virtual quint16 tileSize() const;

    virtual quint8 minZoomLevel(QPointF ll);

    virtual quint8 maxZoomLevel(QPointF ll);

    virtual QString name() const;

    virtual QString tileFileExtension() const;


    void addSourceTop(QSharedPointer<MapTileSource>, qreal opacity = 1.0);
    void addSourceBottom(QSharedPointer<MapTileSource>, qreal opacity = 1.0);

protected:
    virtual void fetchTile(quint32 x,
                           quint32 y,
                           quint8 z);
    
signals:
    
public slots:

private slots:
    void handleTileRetrieved(quint32 x, quint32 y, quint8 z);

private:
    QList<QSharedPointer<MapTileSource> > _childSources;
    QList<qreal> _childOpacities;

    QHash<QString, QMap<quint32, QImage *> * > _pendingTiles;
    
};

#endif // COMPOSITETILESOURCE_H
