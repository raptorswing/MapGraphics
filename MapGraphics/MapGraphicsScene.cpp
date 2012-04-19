#include "MapGraphicsScene.h"


MapGraphicsScene::MapGraphicsScene(QObject * parent)
    : QObject(parent)
{
}

MapGraphicsScene::~MapGraphicsScene()
{
}

void MapGraphicsScene::addObject(MapGraphicsObject *object)
{
    _objects.insert(object);
    this->objectAdded(object);
}

QList<MapGraphicsObject *> MapGraphicsScene::objects() const
{
    QList<MapGraphicsObject *> toRet;

    return toRet;
}
