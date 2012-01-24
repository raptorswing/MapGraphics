#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>

class ZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    void setRange(int min, int max);


protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    
signals:
    void sliderMoved(int value);
    
public slots:
    void setValue(int value);

private:
    QRect sliderRect() const;
    QRect slideBarRect() const;
    int _min;
    int _max;
    int _value;

    qreal _pos;

    bool _dragCanStart;
    
};

#endif // ZOOMWIDGET_H
