#include "ZoomWidget.h"

#include <QSizePolicy>
#include <QPainter>
#include <QMouseEvent>
#include <QtDebug>

ZoomWidget::ZoomWidget(QWidget *parent) :
    QWidget(parent), _min(0), _max(10), _value(5),_pos(0.5), _dragCanStart(false)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred,
                                    QSizePolicy::Preferred));
    this->setFixedSize(this->sizeHint());
}

QSize ZoomWidget::sizeHint() const
{
    return QSize(50,200);
}

void ZoomWidget::setRange(int min, int max)
{
    _min = min;
    _max = max;
    _value = qMin<int>(_value,_max);
    _value = qMax<int>(_value,_min);
}

//protected
void ZoomWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    bool active = (this->rect().contains(this->mapFromGlobal(QCursor::pos()))
                   || _dragCanStart);

    QColor vFillColor;
    QColor vLineColor;
    QColor hFillColor;
    QColor hLineColor;

    if (active)
    {
        vLineColor = QColor(200,200,200,255);
        vFillColor = QColor(255,255,255,255);

        hLineColor = QColor(50,50,50,255);
        hFillColor = QColor(200,200,200,255);
    }
    else
    {
        vLineColor = QColor(200,200,200,255);
        vFillColor = QColor(255,255,255,100);

        hLineColor = QColor(50,50,50,100);
        hFillColor = QColor(200,200,200,100);
    }


    //Draw the vertical line
    painter.setBrush(QBrush(vFillColor));
    QPen verticalLinePen;
    verticalLinePen.setWidth(1);
    verticalLinePen.setColor(vLineColor);
    painter.setPen(verticalLinePen);

    QRect vRect = this->slideBarRect();
    painter.drawRoundedRect(vRect,4.0,4.0);

    //Draw the horizontal line
    painter.setBrush(QBrush(hFillColor));
    QPen horizontalLinePen;
    horizontalLinePen.setWidth(1.0);
    horizontalLinePen.setColor(hLineColor);
    painter.setPen(horizontalLinePen);

    QRect hRect = this->sliderRect();
    painter.drawRoundedRect(hRect,4.0,4.0);
}

//protected
void ZoomWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!this->_dragCanStart)
        return;

    qreal curPos = 1.0 - (qreal) event->pos().y() / this->height();
    curPos = qMin<qreal>(1.0,curPos);
    curPos = qMax<qreal>(0.0,curPos);
    _pos = curPos;

    int newVal = ((_max - _min) * _pos) + _min;
    _value = newVal;

    this->sliderMoved(_value);
    this->update();
}

//protected
void ZoomWidget::mousePressEvent(QMouseEvent * event)
{
    //We only care about the left mouse button
    if (event->button() != Qt::LeftButton)
        return;

    //We care if the mouse is within our magical slider
    QRect sliderRect = this->sliderRect();
    if (!sliderRect.contains(event->pos()))
        return;

    this->_dragCanStart = true;
}

//protected
void ZoomWidget::mouseReleaseEvent(QMouseEvent *)
{
    this->_dragCanStart = false;
}

//protected
void ZoomWidget::enterEvent(QEvent *)
{
    this->update();
}

//protected
void ZoomWidget::leaveEvent(QEvent *)
{
    this->update();
}


//public slot
void ZoomWidget::setValue(int value)
{
    value = qMin<int>(_max,value);
    value = qMax<int>(_min,value);
    _value = value;
    if (!_dragCanStart)
        _pos = (qreal)_value / _max;
    this->update();
}

//private
QRect ZoomWidget::sliderRect() const
{
    const qreal horizontalLineHeight = 10.0;
    const qreal horizontalLineWidth = 20.0;
    const int centerVert = this->height() - this->height()*this->_pos;
    const QRect horizontalLineRect(this->width()/2.0 - horizontalLineWidth/2.0,
                                   centerVert - horizontalLineHeight / 2.0,
                                   horizontalLineWidth,
                                   horizontalLineHeight);
    return horizontalLineRect;
}

//private
QRect ZoomWidget::slideBarRect() const
{
    const qreal verticalLineWidth = 6;
    const QRect verticalLineRect(this->width()/2.0 - verticalLineWidth/2.0,
                         0,
                         verticalLineWidth,
                         this->height());
    return verticalLineRect;
}


