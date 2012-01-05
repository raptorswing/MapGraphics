#ifndef MAPINFOMANAGER_H
#define MAPINFOMANAGER_H

#include "MapTileSource.h"
#include "MapGraphics_global.h"

#include <QSharedPointer>


class MAPGRAPHICSSHARED_EXPORT MapInfoManager
{
private:
    MapInfoManager();

public:
    ~MapInfoManager();
    static MapInfoManager * getInstance();

    void setMapTileSource(QSharedPointer<MapTileSource> newSource);
    QSharedPointer<MapTileSource> getMapTileSource() const;



private:
    static MapInfoManager * instance;

    QSharedPointer<MapTileSource> mapTileSource;
};

#endif // MAPINFOMANAGER_H
