#ifndef POSITION_H
#define POSITION_H

#include <QPointF>
#include <QtDebug>
#include <QVector3D>

#include "MapGraphics_global.h"

class MAPGRAPHICSSHARED_EXPORT Position
{
public:
    Position();
    Position(qreal longitude, qreal latitude, qreal altitude);
    Position(const QPointF& lonLat, qreal altitude);
    Position(const Position& other);
    ~Position();

    bool operator ==(const Position& other);
    bool operator !=(const Position& other);
    Position& operator =(const Position& other);

    qreal longitude() const;
    qreal latitude() const;
    qreal altitude() const;
    QPointF lonLat() const;

    void setLongitude(const qreal& longitude);
    void setLatitude(const qreal& latitude);
    void setAltitude(const qreal& altitude);

    static QVector3D Position2ENU(const Position& refPos, const Position& pos);
    static Position fromENU(const Position& refPos, const QVector3D& enu);

private:
    QPointF _lonLat;
    qreal _altitude;
};

//Non-member method for streaming to qDebug
MAPGRAPHICSSHARED_EXPORT QDebug operator<<(QDebug dbg, const Position& pos);

//Non-member methods for serializing and de-serializing
MAPGRAPHICSSHARED_EXPORT QDataStream& operator<<(QDataStream& stream, const Position& pos);
MAPGRAPHICSSHARED_EXPORT QDataStream& operator>>(QDataStream& stream, Position& pos);

#endif // POSITION_H
