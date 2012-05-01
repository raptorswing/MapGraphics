#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <QTransform>
#include <QVector3D>
#include <QPointF>

#include "MapGraphics_global.h"

class MAPGRAPHICSSHARED_EXPORT Conversions
{
public:
    struct Position
    {
        QPointF lonlat;
        qreal altitude;
    };
public:
    static QVector3D lla2xyz(qreal wlat, qreal wlon, qreal walt);
    static QVector3D lla2xyz(Position lla);
    static Position xyz2lla(QVector3D);
    static Position xyz2lla(qreal x, qreal y, qreal z);

    static QVector3D xyz2enu(QVector3D xyz, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D xyz2enu(QVector3D xyz, Position refLLA);
    static QVector3D xyz2enu(qreal x, qreal y, qreal z, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D xyz2enu(qreal x, qreal y, qreal z, Position refLLA);
    static QVector3D enu2xyz(QVector3D enu, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D enu2xyz(QVector3D enu, Position refLLA);
    static QVector3D enu2xyz(qreal east, qreal north, qreal up, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D enu2xyz(qreal east, qreal north, qreal up, Position refLLA);

    static Position enu2lla(QVector3D enu, qreal reflat, qreal reflon, qreal refalt);
    static Position enu2lla(QVector3D enu, Position refLLA);
    static Position enu2lla(qreal east, qreal north, qreal up, qreal reflat, qreal reflon, qreal refalt);
    static Position enu2lla(qreal east, qreal north, qreal up, Position refLLA);
    static QVector3D lla2enu(qreal lat, qreal lon, qreal alt, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D lla2enu(qreal lat, qreal lon, qreal alt, Position refLLA);
    static QVector3D lla2enu(Position lla, qreal reflat, qreal reflon, qreal refalt);
    static QVector3D lla2enu(Position lla, Position refLLA);

    static QTransform rot(qreal angle, quint32 axis);

    static void test();

};

#endif // CONVERSIONS_H
