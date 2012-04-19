#include "PrivateQGraphicsObject.h"

#include <QtDebug>

#include "guts/Conversions.h"

PrivateQGraphicsObject::PrivateQGraphicsObject(MapGraphicsObject *mgObj,
                                               PrivateQGraphicsInfoSource *infoSource,
                                               QGraphicsItem *parent) :
    QGraphicsObject(parent), _infoSource(infoSource)
{
    this->setMGObj(mgObj);
    this->setZValue(5.0);
}

//pure-virtual from QGraphicsItem
QRectF PrivateQGraphicsObject::boundingRect() const
{
    QRectF toRet(-1.0,-1.0,2.0,2.0);
    if (_mgObj.isNull())
    {
        qWarning() << this << "could not get bounding rect as MapGraphicsObject is null";
        return toRet;
    }

    //Get bounding rect in ENU
    QRectF enuRect = _mgObj->boundingRect();

    //Convert from ENU to lat/lon
    QPointF latLonCenter = _mgObj->pos();
    Conversions::Position latLonCenterPos = {latLonCenter,0.0};
    QPointF leftLatLon = Conversions::enu2lla(enuRect.left(),
                                              0.0,
                                              0.0,
                                              latLonCenterPos).lonlat;
    QPointF upLatLon = Conversions::enu2lla(0.0,
                                            enuRect.top(),
                                            0.0,
                                            latLonCenterPos).lonlat;

    qreal lonWidth = 2.0*(latLonCenter.x() - leftLatLon.x());
    qreal latHeight = 2.0*(upLatLon.y() - latLonCenter.y());

    //Once we've got the rect in lat/lon, we should convert it to scene pixels
    QRectF latLonRect(leftLatLon.x(),upLatLon.y(),lonWidth,latHeight);

    //We need our tile source to do the conversion
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    if (tileSource.isNull())
    {
        qWarning() << this << "can't do bounding box conversion, null tile source.";
        return toRet;
    }

    int zoomLevel = _infoSource->zoomLevel();
    QPointF topLeft = tileSource->ll2qgs(latLonRect.topLeft(),zoomLevel);
    QPointF bottomRight = tileSource->ll2qgs(latLonRect.bottomRight(),zoomLevel);

    toRet = QRectF(topLeft,bottomRight);
    toRet.moveCenter(QPointF(0,0));
    return toRet;
}

//pure-virtual from QGraphicsItem
void PrivateQGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (_mgObj.isNull())
    {
        qWarning() << this << "could not paint as our MapGraphicsObject is null";
        return;
    }

    //Transform painter coordinates to ENU and then have the MapGraphicsObject do its thing
    QRectF enuRect = _mgObj->boundingRect();
    qreal desiredWidthMeters = enuRect.width();
    qreal desiredHeightMeters = enuRect.height();
    QRectF pixelRect = this->boundingRect();
    qreal widthPixels = pixelRect.width();
    qreal heightPixels = pixelRect.height();

    qreal scaleX = widthPixels / desiredWidthMeters;
    qreal scaleY = heightPixels / desiredHeightMeters;

    painter->scale(scaleX,scaleY);

    _mgObj->paint(painter,option,widget);
}

void PrivateQGraphicsObject::handleZoomLevelChanged()
{
    this->handlePosChanged();
}

//private slot
void PrivateQGraphicsObject::handleEnabledChanged()
{
    this->setEnabled(_mgObj->enabled());
}

//private slot
void PrivateQGraphicsObject::handleOpacityChanged()
{
    this->setOpacity(_mgObj->opacity());
}

//private slot
void PrivateQGraphicsObject::handleParentChanged()
{
    //Need more complicated logic here that involves PrivateQGraphicsScene
}

//private slot
void PrivateQGraphicsObject::handlePosChanged()
{
    //Get the position of the object in lat,lon,alt
    QPointF geoPos = _mgObj->pos();

    //If the object has a parent, do stupid stuff here to handle it

    //Convert LLA coordinates to QGraphicsScene coordinates
    QSharedPointer<MapTileSource> tileSource = _infoSource->tileSource();
    if (tileSource.isNull())
        return;

    int zoomLevel = _infoSource->zoomLevel();
    QPointF qgsPos = tileSource->ll2qgs(geoPos,zoomLevel);
    this->setPos(qgsPos);
}

//private slot
void PrivateQGraphicsObject::handleRotationChanged()
{
    this->setRotation(_mgObj->rotation());
}

//private slot
void PrivateQGraphicsObject::handleVisibleChanged()
{
    this->setVisible(_mgObj->visible());
}

//private slot
void PrivateQGraphicsObject::handleZValueChangd()
{
    this->setZValue(_mgObj->zValue());
}

//private slot
void PrivateQGraphicsObject::updateAll()
{
    this->handleEnabledChanged();
    this->handleOpacityChanged();
    this->handleParentChanged();
    this->handlePosChanged();
    this->handleRotationChanged();
    this->handleVisibleChanged();
    this->handleZValueChangd();
}

//private
void PrivateQGraphicsObject::setMGObj(MapGraphicsObject * mgObj)
{
    _mgObj = mgObj;

    if (_mgObj == 0)
        return;

    //Connect signals/slots for all of the changes in the MapGraphicsObject that we must be able to handle
    connect(_mgObj,
            SIGNAL(enabledChanged()),
            this,
            SLOT(handleEnabledChanged()));
    connect(_mgObj,
            SIGNAL(opacityChanged()),
            this,
            SLOT(handleOpacityChanged()));
    connect(_mgObj,
            SIGNAL(parentChanged()),
            this,
            SLOT(handleParentChanged()));
    connect(_mgObj,
            SIGNAL(posChanged()),
            this,
            SLOT(handlePosChanged()));
    connect(_mgObj,
            SIGNAL(rotationChanged()),
            this,
            SLOT(handleRotationChanged()));
    connect(_mgObj,
            SIGNAL(visibleChanged()),
            this,
            SLOT(handleVisibleChanged()));
    connect(_mgObj,
            SIGNAL(zValueChangd()),
            this,
            SLOT(handleZValueChangd()));

    //Get all of the info about the MGObject
    this->updateAll();
}
