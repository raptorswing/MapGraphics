#ifndef CIRCLEOBJECT_H
#define CIRCLEOBJECT_H

#include "MapGraphics_global.h"
#include "MapGraphicsObject.h"

class MAPGRAPHICSSHARED_EXPORT CircleObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit CircleObject(qreal radiusMeters,MapGraphicsObject *parent = 0);

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
signals:
    
public slots:

private:
    qreal _radiusMeters;
    
};

#endif // CIRCLEOBJECT_H
