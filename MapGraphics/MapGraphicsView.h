#ifndef MAPGRAPHICSVIEW_H
#define MAPGRAPHICSVIEW_H

#include <QWidget>
#include <QPointer>
#include <QSharedPointer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QList>

#include "MapGraphicsScene.h"
#include "MapGraphicsObject.h"
#include "MapTileSource.h"
#include "MapGraphics_global.h"
#include "guts/MapTileGraphicsObject.h"

class MAPGRAPHICSSHARED_EXPORT MapGraphicsView : public QWidget
{
    Q_OBJECT
public:
    enum DragMode
    {
        NoDrag,
        ScrollHandDrag,
        RubberBandDrag
    };

    enum ZoomMode
    {
        CenterZoom,
        MouseZoom
    };

public:
    explicit MapGraphicsView(MapGraphicsScene * scene=0, QWidget * parent = 0);
    virtual ~MapGraphicsView();

    void centerOn(const QPointF& pos);
    void centerOn(qreal longitude, qreal latitude);
    void centerOn(const MapGraphicsObject * item);

    QPointF mapToScene(const QPoint viewPos) const;

    MapGraphicsView::DragMode dragMode() const;
    void setDragMode(MapGraphicsView::DragMode);

    MapGraphicsScene * scene() const;
    void setScene(MapGraphicsScene *);

    QSharedPointer<MapTileSource> tileSource() const;
    /**
     * @brief Sets the tile source that this view will pull from.
     * MapGraphicsView does NOT take ownership of the tile source.
     *
     * @param tSource
     */
    void setTileSource(QSharedPointer<MapTileSource> tSource);

    quint8 zoomLevel() const;
    void setZoomLevel(quint8 nZoom, ZoomMode zMode = CenterZoom);

    void zoomIn(ZoomMode zMode = CenterZoom);
    void zoomOut(ZoomMode zMode = CenterZoom);
    
signals:
    
public slots:

protected slots:
    virtual void handleChildViewScrollWheel(QWheelEvent * event);

private slots:
    void renderTiles();

protected:
    void doTileLayout();
    void resetQGSSceneSize();

private:
    QPointer<MapGraphicsScene> _scene;
    QPointer<QGraphicsView> _childView;
    QPointer<QGraphicsScene> _childScene;
    QSharedPointer<MapTileSource> _tileSource;

    QList<MapTileGraphicsObject *> _tileObjects;

    quint8 _zoomLevel;

    DragMode _dragMode;

    
};

#endif // MAPGRAPHICSVIEW_H
