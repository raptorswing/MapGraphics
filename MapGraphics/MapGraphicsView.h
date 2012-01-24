#ifndef MapGraphicsView_H
#define MapGraphicsView_H

#include <QWidget>
#include <QPointer>
#include <QGraphicsView>
#include <QTimer>

#include "guts/MapGraphics_global.h"
#include "guts/MapGraphicsScene.h"
#include "MapGraphicsItem.h"
#include "CompassWidget.h"
#include "ZoomWidget.h"


namespace Ui {
    class MapGraphicsView;
}

class MAPGRAPHICSSHARED_EXPORT MapGraphicsView : public QWidget
{
    Q_OBJECT

//Enum Zone
public:
    enum EdgeMouseMode
    {
        IgnoreEdges,
        ScrollNearEdges,
        ScrollNearAndBeyondEdges
    };

    enum ZoomCenterMode
    {
        PreserveCenter,
        FollowMouse
    };

//Function zone
public:
    explicit MapGraphicsView(QWidget *parent = 0);
    virtual ~MapGraphicsView();

    QGraphicsView * qGraphicsView() const;

    void centerOn(const QPointF & pos);
    void centerOn(qreal x, qreal y);
    void centerOn(const MapGraphicsItem * item);

    void setScene(MapGraphicsScene * scene);
    MapGraphicsScene * scene() const;

    QGraphicsView::DragMode dragMode() const;
    void setDragMode(QGraphicsView::DragMode mode);

    EdgeMouseMode edgeMouseMode() const;
    void setEdgeMouseMode(EdgeMouseMode mode);

    QPoint mapFromScene(const QPointF& point) const;
    QPointF mapToScene(const QPoint& point) const;

public slots:
    void rotate(qreal angle);
    void zoomIn(ZoomCenterMode mode);
    void zoomOut(ZoomCenterMode mode);
    void setZoom(int level,ZoomCenterMode mode = PreserveCenter);

signals:
    void geometryChanged(QRect);

protected:

protected slots:
    virtual void on_graphicsView_hadMousePressEvent(QMouseEvent *);
    virtual void on_graphicsView_hadMouseReleaseEvent(QMouseEvent *);
    virtual void on_graphicsView_hadResizeEvent(QResizeEvent *);
    virtual void on_graphicsView_hadWheelEvent(QWheelEvent *);


private slots:
    void ensureStuffDisplayed();
    void doEdgeMouseScrolling();

private:
    Ui::MapGraphicsView *ui;
    QPointer<MapGraphicsScene> mapGraphicsScene;

    CompassWidget * compassWidget;
    ZoomWidget * zoomWidget;

    EdgeMouseMode currentEdgeMouseMode;
    QTimer * edgeMouseDetectionTimer;
};

#endif // MapGraphicsView_H
