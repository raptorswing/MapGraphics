#ifndef MAPGRAPHICSOBJECT_H
#define MAPGRAPHICSOBJECT_H

#include <QObject>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "MapGraphics_global.h"

class MAPGRAPHICSSHARED_EXPORT MapGraphicsObject : public QObject
{
    Q_OBJECT
public:
    explicit MapGraphicsObject(MapGraphicsObject *parent = 0);
    virtual ~MapGraphicsObject();

    /**
     * @brief You must implement this to return the bounding rect of the object in East,North,Up (ENU)
     * coordinates
     * centered at the pos() of the Object. The units of the rectangle are meters.
     *
     * @return QRectF Bounding rectangle in ENU coordinates. Unit: meters
     */
    virtual QRectF boundingRect() const=0;

    /**
     * @brief Paints the contents of the Object in ENU coordinates --- the same coordinates as boundingRect.
     * You must implement this.
     *
     * @param painter
     * @param option
     * @param widget
     */
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget=0)=0;

    bool enabled() const;
    void setEnabled(bool);

    qreal opacity() const;
    void setOpacity(qreal);

    MapGraphicsObject * parent() const;
    void setParent(MapGraphicsObject *);

    QPointF pos() const;
    void setPos(const QPointF&);

    qreal rotation() const;
    void setRotation(qreal);

    bool visible() const;
    void setVisible(bool);

    qreal longitude() const;
    void setLongitude(qreal);

    qreal latitude() const;
    void setLatitude(qreal);

    qreal zValue() const;
    void setZValue(qreal);

    
signals:
    void enabledChanged();
    void opacityChanged();
    void parentChanged();
    void posChanged();
    void rotationChanged();
    void visibleChanged();
    void zValueChangd();

    
public slots:

private:
    bool _enabled;
    qreal _opacity;
    MapGraphicsObject * _parent;
    QPointF _pos;
    qreal _rotation;
    bool _visible;
    qreal _zValue;
    
};

#endif // MAPGRAPHICSOBJECT_H
