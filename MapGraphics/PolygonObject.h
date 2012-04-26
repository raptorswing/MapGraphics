#ifndef POLYGONOBJECT_H
#define POLYGONOBJECT_H

#include <QPolygonF>
#include <QList>

#include "MapGraphicsObject.h"
#include "MapGraphics_global.h"
class CircleObject;

class MAPGRAPHICSSHARED_EXPORT PolygonObject : public MapGraphicsObject
{
    Q_OBJECT
public:
    explicit PolygonObject(QPolygonF geoPoly, QColor fillColor = QColor(200,200,200,200), QObject *parent = 0);
    virtual ~PolygonObject();

    //pure-virtual from MapGraphicsObject
    QRectF boundingRect() const;

    //pure-virtual from MapGraphicsObject
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    //virtual from MapGraphicsObject
    virtual void setPos(const QPointF &);
    
signals:
    
public slots:

protected:
    //virtual from MapGraphicsObject
    virtual void keyReleaseEvent(QKeyEvent *event);

private slots:
    void handleEditCirclePosChanged();
    void handleAddVertexCircleSelected();

private:
    void fixAddVertexCirclePos();
    CircleObject * constructEditCircle();
    CircleObject * constructAddVertexCircle();
    QPolygonF _geoPoly;
    QColor _fillColor;

    QList<CircleObject *> _editCircles;
    QList<CircleObject *> _addVertexCircles;
    
};

#endif // POLYGONOBJECT_H
