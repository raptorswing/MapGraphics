#include "MGNetworkManager.h"

#include <QNetworkDiskCache>
#include <QDir>
#include <QtDebug>

const QByteArray DEFAULT_USER_AGENT = "MapGraphics";

//static
MGNetworkManager * MGNetworkManager::instance = 0;

//static
MGNetworkManager * MGNetworkManager::getInstance()
{
    if (MGNetworkManager::instance == 0)
        MGNetworkManager::instance = new MGNetworkManager();
    return MGNetworkManager::instance;
}

//protected
MGNetworkManager::MGNetworkManager()
{
    this->manager = new QNetworkAccessManager();

    //Don't enable caching by default
    this->setCachingEnabled(false);

    this->setUserAgent(DEFAULT_USER_AGENT);
}

MGNetworkManager::~MGNetworkManager()
{
    this->manager->deleteLater();
}

QNetworkReply * MGNetworkManager::get(QNetworkRequest &request)
{
    request.setRawHeader("User-Agent",_userAgent);
    QNetworkReply * toRet = this->manager->get(request);
    return toRet;
}

void MGNetworkManager::setCachingEnabled(bool enable)
{
    if (enable == this->cachingEnabled())
        return;
    else if (enable)
    {
        //QNetworkAccessManager takes ownership of the cache object and will delete it
        QNetworkDiskCache * cache = new QNetworkDiskCache();
        cache->setCacheDirectory(QDir::homePath() + "/.MapGraphicsNetCache");
        this->manager->setCache(cache);
    }
    else
    {
        //Set to null
        this->manager->setCache(0);
    }
}

bool MGNetworkManager::cachingEnabled() const
{
    return (this->manager->cache() != 0);
}

void MGNetworkManager::setUserAgent(const QByteArray &agent)
{
    _userAgent = agent;
}

QByteArray MGNetworkManager::userAgent() const
{
    return _userAgent;
}
