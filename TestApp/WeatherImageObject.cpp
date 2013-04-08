#include "WeatherImageObject.h"

WeatherImageObject::WeatherImageObject(const QImage &img,
                                       const QRectF &sizeInMeters,
                                       MapGraphicsObject *parent) :
    MapGraphicsObject(false, parent), _img(img)
{
    this->setWeather(img, sizeInMeters);
}

//pure-virtual from MapGraphicsObject
QRectF WeatherImageObject::boundingRect() const
{
    return _sizeInMeters;
}

//pure-virtual from MapGraphicsObject
void WeatherImageObject::paint(QPainter *painter,
                               const QStyleOptionGraphicsItem *option,
                               QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->drawImage(this->boundingRect(), _img);
}

//public slot
void WeatherImageObject::setWeather(const QImage &img, const QRectF &sizeInMeters)
{
    _img = img;

    const qreal width = sizeInMeters.width();
    const qreal height = sizeInMeters.height();
    _sizeInMeters = QRectF(-0.5 * width,
                           -0.5 * height,
                           width, height);

    this->redrawRequested();
}

//public slot
void WeatherImageObject::setWeather(const QImage &img,
                                    const QRectF &sizeInMeters,
                                    const QPointF &pos)
{
    this->setWeather(img, sizeInMeters);
    this->setPos(pos);
}
