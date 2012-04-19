#include "MapGraphicsObject.h"

MapGraphicsObject::MapGraphicsObject(MapGraphicsObject *parent)
{
    //Set default properties and the parent that was passed as argument
    _enabled = true;
    _opacity = 1.0;
    this->setParent(parent);
    _pos = QPointF(0.0,0.0);
    _rotation = 0.0;
    _visible = true;
    _zValue = 0.0;
}

MapGraphicsObject::~MapGraphicsObject()
{
}

bool MapGraphicsObject::enabled() const
{
    return _enabled;
}

void MapGraphicsObject::setEnabled(bool nEnabled)
{
    _enabled = nEnabled;
    this->enabledChanged();
}

qreal MapGraphicsObject::opacity() const
{
    return _opacity;
}

void MapGraphicsObject::setOpacity(qreal nOpacity)
{
    _opacity = nOpacity;
    this->opacityChanged();
}

MapGraphicsObject *MapGraphicsObject::parent() const
{
    return _parent;
}

void MapGraphicsObject::setParent(MapGraphicsObject * nParent)
{
    _parent = nParent;
    this->parentChanged();
}

QPointF MapGraphicsObject::pos() const
{
    return _pos;
}

void MapGraphicsObject::setPos(const QPointF & nPos)
{
    _pos = nPos;
    this->posChanged();
}

qreal MapGraphicsObject::rotation() const
{
    return _rotation;
}

void MapGraphicsObject::setRotation(qreal nRotation)
{
    _rotation = nRotation;
    this->rotationChanged();
}

bool MapGraphicsObject::visible() const
{
    return _visible;
}

void MapGraphicsObject::setVisible(bool nVisible)
{
    _visible = nVisible;
    this->visibleChanged();
}

qreal MapGraphicsObject::longitude() const
{
    return _pos.x();
}

void MapGraphicsObject::setLongitude(qreal nLongitude)
{
    this->setPos(QPointF(nLongitude,this->pos().y()));
}

qreal MapGraphicsObject::latitude() const
{
    return _pos.y();
}

void MapGraphicsObject::setLatitude(qreal nLatitude)
{
    this->setPos(QPointF(this->pos().x(),nLatitude));
}

qreal MapGraphicsObject::zValue() const
{
    return _zValue;
}

void MapGraphicsObject::setZValue(qreal nZValue)
{
    _zValue = nZValue;
    this->zValueChangd();
}
