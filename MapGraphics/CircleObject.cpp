#include "CircleObject.h"

#include <QtDebug>

CircleObject::CircleObject(qreal radius,bool sizeIsZoomInvariant, MapGraphicsObject *parent) :
    MapGraphicsObject(sizeIsZoomInvariant,parent)
{
    _radiusMeters = qMax<qreal>(radius,0.01);

    this->setFlag(MapGraphicsObject::ObjectIsSelectable);
    this->setFlag(MapGraphicsObject::ObjectIsMovable);
}

CircleObject::~CircleObject()
{
}

QRectF CircleObject::boundingRect() const
{
    return QRectF(-1*_radiusMeters,
                  -1*_radiusMeters,
                  2*_radiusMeters,
                  2*_radiusMeters);
}

void CircleObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setBrush(QColor(200,200,200,150));
    painter->drawEllipse(QPointF(0,0),
                         _radiusMeters,
                         _radiusMeters);
}
