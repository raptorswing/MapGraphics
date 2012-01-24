#ifndef COMPASSWIDGET_H
#define COMPASSWIDGET_H

#include <QWidget>

class CompassWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CompassWidget(QWidget *parent = 0);

    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

signals:
    void rotationChanged(qreal angle);

public slots:

private:
    qreal rotationDegrees;
    bool mouseDragStarted;
    qreal lastDragAngle;
    QPoint clickPos;

};

#endif // COMPASSWIDGET_H
