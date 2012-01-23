#include "MapInfoManager.h"

#include <QThread>

//static
MapInfoManager * MapInfoManager::instance = 0;

//private
MapInfoManager::MapInfoManager()
{
}

//static
MapInfoManager * MapInfoManager::getInstance()
{
    if (MapInfoManager::instance == 0)
        MapInfoManager::instance = new MapInfoManager();
    return MapInfoManager::instance;
}

void MapInfoManager::setMapTileSource(QSharedPointer<MapTileSource> newSource)
{
    this->mapTileSource = newSource;


    MapTileSource * rawSource = newSource.data();
    QThread * thread = new QThread();
    QObject::connect(rawSource,
            SIGNAL(destroyed()),
            thread,
            SLOT(quit()));
    QObject::connect(thread,
            SIGNAL(finished()),
            thread,
            SLOT(deleteLater()));
    rawSource->moveToThread(thread);
    thread->start();
}

QSharedPointer<MapTileSource> MapInfoManager::getMapTileSource() const
{
    return this->mapTileSource;
}
