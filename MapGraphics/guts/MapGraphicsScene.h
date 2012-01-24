#ifndef MAPGRAPHICSSCENE_H
#define MAPGRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QObject>
#include <QList>

#include "MapGraphics_global.h"
#include "MapTileGraphicsItem.h"
#include "MapGraphicsItem.h"

class MAPGRAPHICSSHARED_EXPORT MapGraphicsScene : public QObject
{
    Q_OBJECT
public:
    explicit MapGraphicsScene(QObject *parent = 0);
    virtual ~MapGraphicsScene();

    QGraphicsScene * scene() const;

    quint8 getZoomLevel() const;

    void addItem(MapGraphicsItem * item);

    /**
     * @brief Returns a list of all items in the scene in descending stacking order.
     *
     */
    QList<MapGraphicsItem *> items() const;

    void clearSelection();

    QList<MapGraphicsItem *> selectedItems() const;



protected:



signals:
    void zoomLevelChanged(int zoom);

public slots:
    void setViewerPosition(QPointF centerPoint, QPolygonF viewport);
    void setZoomLevel(int nZoom);

private slots:
    void updateItemsForZoomChange();

private:
    QGraphicsScene * graphicsScene;

    quint8 zoomLevel;
    quint16 tileSize;
    quint16 displayTilesNeeded;
    quint64 tilesOnZoomLevel;
    quint32 tilesPerRow;
    quint32 tilesPerCol;
    QList<MapTileGraphicsItem *> tileDisplays;

    QHash<MapGraphicsItem *, QGraphicsItem *> mapGraphicsItems;
    QHash<QGraphicsItem *,MapGraphicsItem *> qGraphicsItems;
};

#endif // MAPGRAPHICSSCENE_H
