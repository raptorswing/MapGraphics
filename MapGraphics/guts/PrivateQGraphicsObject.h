#ifndef PRIVATEQGRAPHICSOBJECT_H
#define PRIVATEQGRAPHICSOBJECT_H

#include <QGraphicsObject>
#include <QPointer>

#include "MapGraphicsObject.h"

#include "guts/PrivateQGraphicsInfoSource.h"

class PrivateQGraphicsObject : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit PrivateQGraphicsObject(MapGraphicsObject * mgObj,
                                    PrivateQGraphicsInfoSource * infoSource,
                                    QGraphicsItem *parent = 0);

    //pure-virtual from QGraphicsItem
    virtual QRectF boundingRect() const;

    //pure-virtual from QGraphicsItem
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    
signals:
    
public slots:
    void handleZoomLevelChanged();

private slots:
    void handleEnabledChanged();
    void handleOpacityChanged();
    void handleParentChanged();
    void handlePosChanged();
    void handleRotationChanged();
    void handleVisibleChanged();
    void handleZValueChangd();
    void updateAll();


private:
    void setMGObj(MapGraphicsObject *);

    QPointer<MapGraphicsObject> _mgObj;
    PrivateQGraphicsInfoSource * _infoSource;
    
};

#endif // PRIVATEQGRAPHICSOBJECT_H
