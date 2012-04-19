#include "CircleObject.h"

CircleObject::CircleObject(MapGraphicsObject *parent) :
    MapGraphicsObject(parent)
{
}

QRectF CircleObject::boundingRect() const
{
    return QRectF(-50,
                  -50,
                  100,
                  100);
}

void CircleObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->fillRect(this->boundingRect(),Qt::red);
}
