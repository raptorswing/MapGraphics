#include "PolygonObject.h"

#include "guts/Conversions.h"
#include "CircleObject.h"

#include <QtDebug>
#include <QKeyEvent>

PolygonObject::PolygonObject(QPolygonF geoPoly, QColor fillColor, QObject *parent) :
    MapGraphicsObject(parent), _geoPoly(geoPoly), _fillColor(fillColor)
{
    this->setFlag(MapGraphicsObject::ObjectIsMovable);
    this->setFlag(MapGraphicsObject::ObjectIsSelectable);
    this->setFlag(MapGraphicsObject::ObjectIsFocusable);
    this->setPos(_geoPoly.boundingRect().center());
}

PolygonObject::~PolygonObject()
{
    foreach(CircleObject * circle, _editCircles)
        circle->deleteLater();
    _editCircles.clear();
}

//pure-virtual from MapGraphicsObject
QRectF PolygonObject::boundingRect() const
{
    QRectF latLonRect = _geoPoly.boundingRect();
    QPointF latLonCenter = latLonRect.center();
    Conversions::Position latLonCenterPos = {latLonCenter,0.0};
    Conversions::Position topLeftPos = {latLonRect.topLeft(),0.0};
    Conversions::Position bottomRightPos = {latLonRect.bottomRight(),0.0};

    QPointF topLeftENU = Conversions::lla2enu(topLeftPos,latLonCenterPos).toPointF();
    QPointF bottomRightENU = Conversions::lla2enu(bottomRightPos,latLonCenterPos).toPointF();

    return QRectF(topLeftENU,bottomRightENU);
}

//pure-virtual from MapGraphicsObject
void PolygonObject::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setRenderHint(QPainter::Antialiasing,true);

    QPolygonF enuPoly;

    Conversions::Position latLonCenterPos = {_geoPoly.boundingRect().center(),0};
    foreach(QPointF latLon, _geoPoly)
    {
        Conversions::Position latLonPos = {latLon,0};
        QPointF enu = Conversions::lla2enu(latLonPos,latLonCenterPos).toPointF();
        enuPoly << enu;
    }

    painter->setBrush(_fillColor);
    painter->drawPolygon(enuPoly);

    if (_editCircles.isEmpty())
    {
        //Create objects to edit the polygon!
        for (int i = 0; i < _geoPoly.size(); i++)
        {
            CircleObject * circle = new CircleObject(8);
            this->newObjectGenerated(circle);
            circle->setPos(_geoPoly.at(i));
            _editCircles.append(circle);
            connect(circle,
                    SIGNAL(posChanged()),
                    this,
                    SLOT(handleEditCirclePosChanged()));
        }
    }
}

void PolygonObject::setPos(const QPointF & nPos)
{
    /*
      If the new position moved the object from the center of the bounding box made by the geo poly
      then we need to move the geo poly
    */
    if (nPos != _geoPoly.boundingRect().center())
    {
        const QPointF diff = nPos - this->pos();
        //_geoPoly.translate(diff);

        //We should also move our edit handles
        foreach(CircleObject * circle, _editCircles)
            circle->setPos(circle->pos() + diff);
    }

    MapGraphicsObject::setPos(nPos);
}

//protected
//virtual from MapGraphicsObject
void PolygonObject::keyReleaseEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Delete))
    {
        this->deleteLater();
        event->accept();
    }
    else
        event->ignore();
}

//private slot
void PolygonObject::handleEditCirclePosChanged()
{
    QObject * sender = QObject::sender();
    if (sender == 0)
        return;
    CircleObject * circle = qobject_cast<CircleObject *>(sender);
    if (circle == 0)
        return;

    int index = _editCircles.indexOf(circle);
    if (index == -1)
        return;

    QPointF newPos = circle->pos();
    _geoPoly.replace(index,newPos);
    this->setPos(_geoPoly.boundingRect().center());
}
