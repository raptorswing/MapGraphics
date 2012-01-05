#include "CompassWidget.h"

#include <QMouseEvent>
#include <QPainter>
#include <QVector2D>
#include <QtDebug>
#include <cmath>

CompassWidget::CompassWidget(QWidget *parent) :
    QWidget(parent), rotationDegrees(0.0), mouseDragStarted(false)
{
    this->setFixedSize(this->sizeHint());
}


QSize CompassWidget::sizeHint() const
{
    return QSize(100,100);
}

//protected
void CompassWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //painter.fillRect(this->rect(),Qt::black);

    bool active = this->rect().contains(this->mapFromGlobal(QCursor::pos()));

    int side = qMin<int>(this->width(),
                         this->height());
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(this->width() / 2,
                      this->height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    painter.rotate(this->rotationDegrees);

    //Create a clipping path for the center of the circle
    QPainterPath path1;
    path1.addRect(-100,-100,200,200);

    QPainterPath path2;
    path2.addEllipse(QPoint(0,0),64,64);

    QPainterPath path = path1.subtracted(path2);
    painter.setClipPath(path);

    QColor alphaWhite;
    if (active)
        alphaWhite = QColor(255,255,255,225);
    else
        alphaWhite = QColor(255,255,255,150);


    //Draw the outer ellipse
    QPen pen(alphaWhite);
    pen.setWidth(8);
    painter.setPen(pen);
    if (active)
        painter.setBrush(QBrush(QColor(255,255,255,150)));
    else
        painter.setBrush(QBrush(QColor(255,255,255,100)));
    painter.drawEllipse(QPoint(0,0),
                        95,95);


    //draw the inner ellipse
    pen.setWidth(3);
    painter.setPen(pen);
    painter.drawEllipse(QPoint(0,0),
                        65,65);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRoundedRect(-11,-90,22,22,6,6);


    QFont font = painter.font();
    font.setPixelSize(18);
    font.setBold(true);
    font.setStyleHint(QFont::Times);
    painter.setFont(font);

    pen.setColor(Qt::black);
    painter.setPen(pen);

    QRect northRect(-10,-90,23,22);
    painter.drawText(northRect,
                     Qt::AlignCenter | Qt::AlignVCenter,
                     "N");


    if (!active)
        return;
    pen.setWidth(5);
    pen.setColor(QColor(150,150,150,50));
    painter.setPen(pen);
    painter.setBrush(QBrush(Qt::gray));

    painter.rotate(90.0);
    painter.drawEllipse(QPoint(-5,-80),1,1);
    painter.drawEllipse(QPoint(5,-80),1,1);
    painter.rotate(90.0);
    painter.drawEllipse(QPoint(-5,-80),1,1);
    painter.drawEllipse(QPoint(5,-80),1,1);
    painter.rotate(90.0);
    painter.drawEllipse(QPoint(-5,-80),1,1);
    painter.drawEllipse(QPoint(5,-80),1,1);
}

//protected
void CompassWidget::mouseMoveEvent(QMouseEvent * event)
{
    if (!this->mouseDragStarted)
        return;

    int side = qMin<int>(this->width(),
                         this->height());
    QPoint center(side/2,side/2);
    QPoint mousePos = event->pos();
    QPoint diff = mousePos - center;

    //calculate angle to 0pi
    qreal angle = (180.0/3.14159265)*atan2((double)diff.y(),(double)diff.x());
    angle += 90.0;
    //qDebug() << angle;


    this->rotationDegrees += ((angle - this->lastDragAngle));
    this->rotationChanged((angle - this->lastDragAngle));
    this->lastDragAngle = angle;
    this->update();
}

//protected
void CompassWidget::mousePressEvent(QMouseEvent * event)
{
    //Find out if its in our magic ring. If so, we can start a "drag" to do rotation
    int side = qMin<int>(this->width(),
                         this->height());
    QPoint center(side/2,side/2);
    QPoint mousePos = event->pos();


    QPoint diff =  mousePos - center;
    QVector2D vector(diff.x(),diff.y());
    qreal length = vector.length();
    if (length < 30 || length > 50)
        return;

    this->clickPos = event->pos();


    this->mouseDragStarted = true;
    //calculate angle to 0pi
    qreal angle = (180.0/3.14159265)*atan2((double)diff.y(),(double)diff.x());
    angle += 90.0;

    this->lastDragAngle = angle;
}

//protected
void CompassWidget::mouseReleaseEvent(QMouseEvent *)
{
    this->mouseDragStarted = false;
}

//protected
void CompassWidget::enterEvent(QEvent *)
{
    this->update();
}

//protected
void CompassWidget::leaveEvent(QEvent *)
{
    this->update();
}

//protected
void CompassWidget::resizeEvent(QResizeEvent *)
{
    /*
    int side = qMin(width(), height());
    QRegion rect(0,0,width(),height());
    QRegion masked(width() / 2 - (side-50) / 2,
                   height() / 2 - (side-50) / 2,
                   side-50,
                   side-50, QRegion::Ellipse);
    QRegion mask = rect - masked;
    setMask(mask);
    */
}


