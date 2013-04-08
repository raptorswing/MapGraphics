#ifndef WEATHERMANAGER_H
#define WEATHERMANAGER_H

#include <QObject>
#include <QPointer>
#include <QNetworkAccessManager>
#include <QSet>
#include <QPair>
#include <QVector2D>
#include <QPointF>

#include "MapGraphicsScene.h"
#include "WeatherImageObject.h"

class WeatherManager : public QObject
{
    Q_OBJECT
public:
    explicit WeatherManager(MapGraphicsScene *scene, QObject *parent = 0);
    ~WeatherManager();
    
signals:
    
public slots:
    void doWeatherUpdate();

private slots:
    void handleRequestFinished(QNetworkReply * reply);
    void handleIndexFinished(QNetworkReply *reply);
    void handleWorldFileFinished(QNetworkReply * reply);
    void handleGifFinished(QNetworkReply * reply);

private:
    WeatherImageObject * _getImageObject(const QString &url);
    void _clearImageObject(const QString& url);
    QPointer<MapGraphicsScene> _scene;
    QNetworkAccessManager * _network;
    
    QSet<QNetworkReply *> _expectingIndex;
    QSet<QNetworkReply *> _expectingWorldFile;
    QSet<QNetworkReply *> _expectingGIF;

    QHash<QNetworkReply *, QPair<QVector2D, QPointF> > _expectingGIFParams;

    QHash<QString, WeatherImageObject*> _displayObjects;
};

#endif // WEATHERMANAGER_H
