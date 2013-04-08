#ifndef WEATHERIMAGEOBJECT_H
#define WEATHERIMAGEOBJECT_H

#include "MapGraphicsObject.h"

#include <QImage>

class WeatherImageObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit WeatherImageObject(const QImage& img,
                                const QRectF& sizeInMeters,
                                MapGraphicsObject *parent = 0);

    //pure-virtual from MapGraphicsObject
    virtual QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    virtual void paint(QPainter * painter,
                       const QStyleOptionGraphicsItem * option,
                       QWidget * widget=0);
    
signals:
    
public slots:
    void setWeather(const QImage& img, const QRectF& sizeInMeters);
    void setWeather(const QImage& img, const QRectF& sizeInMeters, const QPointF& pos);

private:
    QImage _img;
    QRectF _sizeInMeters;
    
};

#endif // WEATHERIMAGEOBJECT_H
