#ifndef MAPTILEGRAPHICSITEM_H
#define MAPTILEGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QtGlobal>
#include <QPixmap>
#include <QObject>
#include <QPointer>

class MapTileGraphicsItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    MapTileGraphicsItem(quint16 tileSize=256);
    virtual ~MapTileGraphicsItem();

    virtual QRectF boundingRect() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    quint16 getTileSize() const;

    void setTileRequestInfo(quint16 x, quint16 y, quint16 z);

private slots:
    void handleTileRetrieved(quint16 x, quint16 y, quint16 z);

signals:
    void tileRequested(quint16 x, quint16 y, quint16 z);

private:
    quint16 tileSize;
    QPixmap * tile;
    quint16 tileX;
    quint16 tileY;
    quint8 tileZoom;

    //quint32 lastTileRequestID;
    bool havePendingRequest;


};

#endif // MAPTILEGRAPHICSITEM_H
