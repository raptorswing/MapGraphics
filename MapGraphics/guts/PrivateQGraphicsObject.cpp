#include "PrivateQGraphicsObject.h"

PrivateQGraphicsObject::PrivateQGraphicsObject(MapGraphicsObject *mgObj, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    this->setMGObj(mgObj);
}

//pure-virtual from QGraphicsItem
QRectF PrivateQGraphicsObject::boundingRect() const
{
    return QRectF(-1.0,-1.0,2.0,2.0);
}

//pure-virtual from QGraphicsItem
void PrivateQGraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
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
