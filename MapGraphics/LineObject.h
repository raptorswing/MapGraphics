#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#include "MapGraphics_global.h"
#include "Position.h"
#include "MapGraphicsObject.h"

class MAPGRAPHICSSHARED_EXPORT LineObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit LineObject(const Position& endA,
                        const Position& endB,
                        MapGraphicsObject *parent = 0);
    virtual ~LineObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
signals:
    
public slots:
    void setEndPointA(const Position& pos);
    void setEndPointB(const Position& pos);
    void setEndPoints(const Position& a,
                      const Position& b);

private slots:
    void updatePositionFromEndPoints();

private:
    Position _a;
    Position _b;
    
};

#endif // LINEOBJECT_H
