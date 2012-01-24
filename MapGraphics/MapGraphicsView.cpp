#include "MapGraphicsView.h"
#include "ui_MapGraphicsView.h"

#include <QtDebug>
#include <QWheelEvent>
#include <QTimer>
#include <QDateTime>
#include <QPainter>
#include <QMouseEvent>
#include <QScrollBar>
#include <QDockWidget>


#include "guts/MapInfoManager.h"

MapGraphicsView::MapGraphicsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MapGraphicsView),
    currentEdgeMouseMode(IgnoreEdges),
    edgeMouseDetectionTimer(0)
{
    ui->setupUi(this);

    QTimer * timer = new QTimer(this);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(ensureStuffDisplayed()));
    timer->start(250);

    qsrand(QDateTime::currentMSecsSinceEpoch());
    //this->rotate(qrand() % 360);


    //Create compass widget
    this->compassWidget = new CompassWidget(this);
    //No title bar
    this->compassWidget->setWindowFlags(Qt::CustomizeWindowHint);
    //No frame
    this->compassWidget->setWindowFlags(Qt::FramelessWindowHint);
    this->compassWidget->setAttribute(Qt::WA_TranslucentBackground);
    connect(this,
            SIGNAL(destroyed()),
            this->compassWidget,
            SLOT(deleteLater()));
    this->compassWidget->move(QPoint(25,25));
    this->compassWidget->show();

    connect(this->compassWidget,
            SIGNAL(rotationChanged(qreal)),
            this,
            SLOT(rotate(qreal)));

    //Create zoom widget
    this->zoomWidget = new ZoomWidget(this);
    this->zoomWidget->move(QPoint(50,125 + 10));
    this->zoomWidget->show();
    this->zoomWidget->setRange(0,17);
    this->zoomWidget->setValue(0);

    connect(this->zoomWidget,
            SIGNAL(sliderMoved(int)),
            this,
            SLOT(setZoom(int)));
}

MapGraphicsView::~MapGraphicsView()
{
    delete this->compassWidget;
    delete ui;
}

QGraphicsView * MapGraphicsView::qGraphicsView() const
{
    return this->ui->graphicsView;
}

void MapGraphicsView::centerOn(const QPointF &pos)
{
    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
    this->qGraphicsView()->centerOn(tileSource->scenePixelFromCoordinate(pos,this->scene()->getZoomLevel()));
}

void MapGraphicsView::centerOn(qreal x, qreal y)
{
    this->centerOn(QPointF(x,y));
}

void MapGraphicsView::centerOn(const MapGraphicsItem * item)
{
    Q_UNUSED(item)
}

void MapGraphicsView::setScene(MapGraphicsScene *scene)
{
    this->mapGraphicsScene = scene;
    this->qGraphicsView()->setScene(scene->scene());

    if (this->zoomWidget != 0)
    {
        connect(scene,
                SIGNAL(zoomLevelChanged(int)),
                this->zoomWidget,
                SLOT(setValue(int)));
    }
}

MapGraphicsScene * MapGraphicsView::scene() const
{
    return this->mapGraphicsScene;
}


//public slot
void MapGraphicsView::rotate(qreal angle)
{
    this->qGraphicsView()->rotate(angle);
}

//public slot
void MapGraphicsView::zoomIn(MapGraphicsView::ZoomCenterMode mode)
{
    this->setZoom(this->scene()->getZoomLevel()+1,mode);
}

//public slot
void MapGraphicsView::zoomOut(MapGraphicsView::ZoomCenterMode mode)
{
    this->setZoom(this->scene()->getZoomLevel()-1,mode);
}

//public slot
void MapGraphicsView::setZoom(int level, MapGraphicsView::ZoomCenterMode mode)
{
    int oldZoom = this->scene()->getZoomLevel();
    if (level == oldZoom)
        return;

    /*
      Find out where the mouse was and store that information so we can re-center
      after zooming in/out.
    */
    QPointF  centerGeoPos = this->mapToScene(QPoint(this->width()/2,this->height()/2));
    QGraphicsView * view = this->qGraphicsView();
    QPointF mousePoint = view->mapToScene(this->mapFromGlobal(QCursor::pos()));
    QRectF sceneRect = view->sceneRect();
    float xRatio = mousePoint.x() / sceneRect.width();
    float yRatio = mousePoint.y() / sceneRect.height();
    QPointF centerPos = view->mapToScene(QPoint(this->width()/2,this->height()/2));
    QPointF offset = mousePoint - centerPos;

    //Get the maximum and minimum zoom levels
    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
    const int maxZoom = tileSource->getMaxZoomLevel();
    const int minZoom = tileSource->getMinZoomLevel();



    QTransform trans = this->qGraphicsView()->transform();

    //Does "overzoom" if we're already zoomed in to our max zoom
    if (oldZoom == maxZoom && trans.m11() < 10 && trans.m22() < 10 && oldZoom < level)
    {
        this->qGraphicsView()->scale((trans.m11() + 0.1)/trans.m11(),(trans.m22() + 0.1)/trans.m22());
        return;
    }
    //Does zooming out of "overzoom"
    else if (trans.m11() > 1 && trans.m22() > 1 && oldZoom > level)
    {
        this->qGraphicsView()->scale((trans.m11() - 0.1)/trans.m11(),(trans.m22() - 0.1)/trans.m22());
        return;
    }


    quint8 newZoom = oldZoom;
    if (level > oldZoom)
        newZoom = qMin<quint8>(oldZoom + 1, maxZoom);
    else if (oldZoom > minZoom)
        newZoom = oldZoom - 1;

    //The scene will do everything necessary with tiles to handle the zoom in/out
    this->scene()->setZoomLevel(newZoom);

    //Center on the same position as before the zoom change
    sceneRect = view->sceneRect();
    mousePoint = QPointF(sceneRect.width()*xRatio,
                         sceneRect.height()*yRatio) - offset;
    if (mode == FollowMouse)
        view->centerOn(mousePoint);
    else if (mode == PreserveCenter)
        this->centerOn(centerGeoPos);

    this->ensureStuffDisplayed();
}

QGraphicsView::DragMode MapGraphicsView::dragMode() const
{
    return this->qGraphicsView()->dragMode();
}

void MapGraphicsView::setDragMode(QGraphicsView::DragMode mode)
{
    this->qGraphicsView()->setDragMode(mode);
}


MapGraphicsView::EdgeMouseMode MapGraphicsView::edgeMouseMode() const
{
    return this->currentEdgeMouseMode;
}

void MapGraphicsView::setEdgeMouseMode(MapGraphicsView::EdgeMouseMode mode)
{
    if (this->currentEdgeMouseMode == mode)
        return;

    this->currentEdgeMouseMode = mode;

    if (mode == ScrollNearEdges || mode == ScrollNearAndBeyondEdges)
    {
        this->edgeMouseDetectionTimer = new QTimer(this);
        connect(this->edgeMouseDetectionTimer,
                SIGNAL(timeout()),
                this,
                SLOT(doEdgeMouseScrolling()));
        this->edgeMouseDetectionTimer->start(25);
        this->ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        this->ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    else
    {
        if (this->edgeMouseDetectionTimer != 0)
        {
            this->edgeMouseDetectionTimer->deleteLater();
            this->edgeMouseDetectionTimer = 0;
        }
        this->ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        this->ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    }
}

QPoint MapGraphicsView::mapFromScene(const QPointF &point) const
{
    /*
      We get the MapGraphicsScene coordinates (lat/lon) and convert them to
      QGraphicsScene coordinates
      */
    const quint8 zoomLevel = this->scene()->getZoomLevel();
    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();
    QPointF qGraphicsSceneCoordinates = tileSource->scenePixelFromCoordinate(point,zoomLevel);

    //Now that we have the QGraphicsScene coordinates we use QGraphicsView's mapFromScene to get the result
    return this->ui->graphicsView->mapFromScene(qGraphicsSceneCoordinates);
}

QPointF MapGraphicsView::mapToScene(const QPoint &point) const
{
    //Given the QGraphicsView coordinates. Map to QGraphicsScene coordinates
    QPointF qGraphicsSceneCoordinates = this->ui->graphicsView->mapToScene(point);

    //Now convert to MapGraphicsScene coordinates
    const quint8 zoomLevel = this->scene()->getZoomLevel();
    QSharedPointer<MapTileSource> tileSource = MapInfoManager::getInstance()->getMapTileSource();

    return tileSource->coordinateFromScenePixel(qGraphicsSceneCoordinates,zoomLevel);
}

//protected slot
void MapGraphicsView::on_graphicsView_hadMousePressEvent(QMouseEvent * event)
{
    qDebug() << "Default view mouse press" << event->pos() << this->mapToScene(event->pos());
    event->ignore();
}

//protected slot
void MapGraphicsView::on_graphicsView_hadMouseReleaseEvent(QMouseEvent * event)
{
    qDebug() << "Default view mouse release" << event->pos() << this->mapToScene(event->pos());
    event->ignore();
}

//protected slot
void MapGraphicsView::on_graphicsView_hadResizeEvent(QResizeEvent *)
{
    this->ensureStuffDisplayed();
}

//protected slot
void MapGraphicsView::on_graphicsView_hadWheelEvent(QWheelEvent * event)
{
    if (event->delta() > 0)
        this->zoomIn(FollowMouse);
    else
        this->zoomOut(FollowMouse);
}

//private slot
void MapGraphicsView::ensureStuffDisplayed()
{
    if (this->scene() == 0)
        return;

    QGraphicsView * view = this->qGraphicsView();
    const QPointF centerPoint = view->mapToScene(view->width()/2,
                                                 view->height()/2);

    QPolygon viewportPolygon;
    viewportPolygon << QPoint(0,0) << QPoint(0,view->height()) << QPoint(view->width(),view->height()) << QPoint(view->width(),0);

    const QPolygonF viewportPolygonMapped = view->mapToScene(viewportPolygon);

    this->scene()->setViewerPosition(centerPoint,viewportPolygonMapped);
}

//private slot
void MapGraphicsView::doEdgeMouseScrolling()
{
    const quint8 margin = 25;
    const quint8 multiplier = 8;
    QPoint pos = this->mapFromGlobal(QCursor::pos());

    qint32 dx = 0;
    qint32 dy = 0;


    QRect rect = this->rect();

    /*
      If we're only in the NEAR (not beyond) mode, then we don't move if
      the mouse is outside the widget/window
    */

    if (this->edgeMouseMode() == ScrollNearEdges && !rect.contains(pos))
        return;


    //Right edge
    if (pos.x() > rect.right() - margin)
        dx = 1;
    //Left edge
    else if (pos.x() < rect.left() + margin)
        dx = -1;

    //Bottom edge
    if (pos.y() > rect.bottom() - margin)
        dy = 1;
    //Top edge
    else if (pos.y() < rect.top() + margin)
        dy = -1;

    if (dx == 0 && dy == 0)
        return;

    dx *= multiplier;
    dy *= multiplier;

    //Move the scrollbars forcibly, brooking no nonsense from them
    QScrollBar * horiz = this->ui->graphicsView->horizontalScrollBar();
    QScrollBar * vert = this->ui->graphicsView->verticalScrollBar();
    horiz->setValue(horiz->value()+ dx);
    vert->setValue(vert->value() + dy);
}

