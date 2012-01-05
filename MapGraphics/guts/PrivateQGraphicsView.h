#ifndef PRIVATEQGRAPHICSVIEW_H
#define PRIVATEQGRAPHICSVIEW_H

#include <QGraphicsView>

class PrivateQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PrivateQGraphicsView(QWidget *parent = 0);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

signals:
    void hadWheelEvent(QWheelEvent * event);
    void hadResizeEvent(QResizeEvent * event);
    void hadMousePressEvent(QMouseEvent * event);
    void hadMouseReleaseEvent(QMouseEvent * event);

};

#endif // PRIVATEQGRAPHICSVIEW_H
