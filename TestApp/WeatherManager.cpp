#include "WeatherManager.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtDebug>
#include <QRegExp>
#include <QTimer>

#include "guts/Conversions.h"

const QUrl SHORT_RANGE_REFLECTIVITY_DIR = QUrl("http://radar.weather.gov/ridge/RadarImg/N0R/");
const QString WORLD_FILE_REGEX_STRING = "<a href=\"([A-Z]{3}_[^\"]*.gfw)\">";
const int updateIntervalMS = 5 * 60 * 1000; //Every five minutes

WeatherManager::WeatherManager(MapGraphicsScene *scene, QObject *parent) :
    QObject(parent), _scene(scene)
{
    _network = new QNetworkAccessManager(this);
    connect(_network,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(handleRequestFinished(QNetworkReply*)));

    QTimer * timer = new QTimer(this);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(doWeatherUpdate()));
    timer->start(updateIntervalMS);

    QTimer::singleShot(0, this, SLOT(doWeatherUpdate()));
}

WeatherManager::~WeatherManager()
{
}

//public slot
void WeatherManager::doWeatherUpdate()
{
    qDebug() << "Update!";
    //Get the list of radar images
    QNetworkRequest indexRequest(SHORT_RANGE_REFLECTIVITY_DIR);
    QNetworkReply * reply = _network->get(indexRequest);
    _expectingIndex.insert(reply);

}

//private slot
void WeatherManager::handleRequestFinished(QNetworkReply *reply)
{
    if (_expectingIndex.contains(reply))
    {
        _expectingIndex.remove(reply);
        this->handleIndexFinished(reply);
    }
    else if (_expectingWorldFile.contains(reply))
    {
        _expectingWorldFile.remove(reply);
        this->handleWorldFileFinished(reply);
    }
    else if (_expectingGIF.contains(reply))
    {
        _expectingGIF.remove(reply);
        this->handleGifFinished(reply);
    }
    else
    {
        qWarning() << this << "received unkown reply";
        reply->deleteLater();
    }
}

//private slot
void WeatherManager::handleIndexFinished(QNetworkReply * reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error retrieving index" << reply->request().url() << ":" << reply->errorString();
        return;
    }

    const QByteArray data = reply->readAll();
    const QString dataString = data;

    QRegExp worldFileFinder(WORLD_FILE_REGEX_STRING, Qt::CaseSensitive);

    int pos = 0;
    while ((pos = worldFileFinder.indexIn(dataString, pos)) != -1)
    {
        pos += worldFileFinder.matchedLength();

        //Build the absolute URL of the world file
        const QUrl worldFileRelativeURL(worldFileFinder.cap(1));
        const QUrl worldFileAbsoluteURL = SHORT_RANGE_REFLECTIVITY_DIR.resolved(worldFileRelativeURL);

//        if (!worldFileFinder.cap(1).contains("MTX"))
//            continue;

        //Make the request for the world file
        QNetworkRequest worldFileRequest(worldFileAbsoluteURL);
        _expectingWorldFile.insert(_network->get(worldFileRequest));
    }

}

//private slot
void WeatherManager::handleWorldFileFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error retrieving worldfile" << reply->request().url() << ":" << reply->errorString();
        return;
    }

    QByteArray data = reply->readAll();
    data.replace('\r', "");

    const QList<QByteArray> lines = data.split('\n');
    if (lines.size() < 6)
    {
        qWarning() << "Wrong number of lines in worldfile" << lines.size();
        return;
    }

    bool ok;
    const qreal lonPerPixel = lines.at(0).toDouble(&ok);
    if (!ok)
    {
        qWarning() << "Bad lonPerPixel in world file";
        return;
    }

    const qreal latPerPixel = qAbs<qreal>(lines.at(3).toDouble(&ok));
    if (!ok)
    {
        qWarning() << "Bad latPerPixel in world file";
        return;
    }

    const qreal lon = lines.at(4).toDouble(&ok);
    if (!ok)
    {
        qWarning() << "Bad longitude in world file";
        return;
    }

    const qreal lat = lines.at(5).toDouble(&ok);
    if (!ok)
    {
        qWarning() << "Bad latitude in world file";
        return;
    }

    const QVector2D perPixels(lonPerPixel, latPerPixel);
    const QPointF topLeftLonLat(lon, lat);

    const QUrl gifURL(reply->request().url().toString().replace("gfw", "gif"));

    const QNetworkRequest gifRequest(gifURL);
    QNetworkReply * gifReply = _network->get(gifRequest);
    _expectingGIF.insert(gifReply);
    _expectingGIFParams.insert(gifReply, QPair<QVector2D, QPointF>(perPixels, topLeftLonLat));
}

void WeatherManager::handleGifFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    const QString reqString = reply->url().toString();

    if (reply->error() != QNetworkReply::NoError)
    {
        qWarning() << "Error retrieving gif" << reply->request().url() << ":" << reply->errorString();
        _clearImageObject(reqString);
        return;
    }

    if (!_expectingGIFParams.contains(reply))
    {
        qWarning() << "Unknown parameters for gif" << reply->request().url();
        _clearImageObject(reqString);
        return;
    }
    const QPair<QVector2D, QPointF> pair = _expectingGIFParams.take(reply);

    const QImage img = QImage::fromData(reply->readAll());
    if (img.isNull())
    {
        qWarning() << "Failed to parse gif from" << reply->request().url();
        _clearImageObject(reqString);
        return;
    }

    const int imgWidth = img.width();
    const int imgHeight = img.height();

    const qreal widthLon = imgWidth * pair.first.x();
    const qreal heightLat = imgHeight * pair.first.y();

    const QPointF centerLonLat(pair.second.x() + widthLon / 2.0,
                               pair.second.y() - heightLat / 2.0);

    const qreal lonPerMeter = Conversions::degreesLonPerMeter(centerLonLat.y());
    const qreal latPerMeter = Conversions::degreesLatPerMeter(centerLonLat.y());

    const QRectF metersRect(0, 0,
                            widthLon / lonPerMeter,
                            heightLat / latPerMeter);

    WeatherImageObject * imgObj = _getImageObject(reqString);
    imgObj->setWeather(img, metersRect, centerLonLat);
    _scene->addObject(imgObj);
}

//private
WeatherImageObject *WeatherManager::_getImageObject(const QString& url)
{
    WeatherImageObject * toRet;
    if (!_displayObjects.contains(url))
    {
        WeatherImageObject * newObj = new WeatherImageObject(QImage(), QRectF(0,0,5,5));
        _displayObjects.insert(url, newObj);
        newObj->setOpacity(0.4);
    }
    toRet = _displayObjects.value(url);

    return toRet;
}

//private
void WeatherManager::_clearImageObject(const QString &url)
{
    if (!_displayObjects.contains(url))
        return;

    WeatherImageObject * obj = _displayObjects.take(url);
    obj->deleteLater();
}
