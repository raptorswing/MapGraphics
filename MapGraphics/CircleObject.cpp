#include "CircleObject.h"

CircleObject::CircleObject(qreal radiusMeters,MapGraphicsObject *parent) :
    MapGraphicsObject(parent)
{
    _radiusMeters = qMax<qreal>(radiusMeters,0.01);

    this->setFlag(MapGraphicsObject::ObjectIsSelectable);
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
    painter->drawEllipse(QPointF(0,0),
                         _radiusMeters,
                         _radiusMeters);
}
