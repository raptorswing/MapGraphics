#include "MGNetworkManager.h"

#include <QNetworkDiskCache>
#include <QDir>
#include <QtDebug>


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

}

MGNetworkManager::~MGNetworkManager()
{
    this->manager->deleteLater();
}

QNetworkReply * MGNetworkManager::get(const QNetworkRequest &request)
{
    return this->manager->get(request);
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
