#include "PrivateQGraphicsScene.h"

#include <QtDebug>

PrivateQGraphicsScene::PrivateQGraphicsScene(MapGraphicsScene * mgScene,
                                             PrivateQGraphicsInfoSource *infoSource,
                                             QObject *parent) :
    QGraphicsScene(parent), _infoSource(infoSource)
{
    this->setMapGraphicsScene(mgScene);
}

//private slot
void PrivateQGraphicsScene::handleMGObjectAdded(MapGraphicsObject * added)
{
    PrivateQGraphicsObject * qgObj = new PrivateQGraphicsObject(added,_infoSource);
    this->addItem(qgObj);

    //We need a mapping of MapGraphicsObject : QGraphicsObject, so put this in the map
    _mgToqg.insert(added,qgObj);
}

//private slot
void PrivateQGraphicsScene::handleMGObjectRemoved(MapGraphicsObject * removed)
{
    if (!_mgToqg.contains(removed))
    {
        qDebug() << "There is no QGraphicsObject in the scene for" << removed;
        return;
    }

    PrivateQGraphicsObject * qgObj = _mgToqg.take(removed);
    this->removeItem(qgObj);
    delete qgObj;
}

void PrivateQGraphicsScene::handleZoomLevelChanged()
{
    foreach(PrivateQGraphicsObject * obj, _mgToqg.values())
        obj->handleZoomLevelChanged();
}

//private
void PrivateQGraphicsScene::setMapGraphicsScene(MapGraphicsScene *mgScene)
{
    _mgScene = mgScene;

    if (_mgScene.isNull())
        return;

    connect(_mgScene,
            SIGNAL(objectAdded(MapGraphicsObject*)),
            this,
            SLOT(handleMGObjectAdded(MapGraphicsObject*)));
    connect(_mgScene,
            SIGNAL(objectRemoved(MapGraphicsObject*)),
            this,
            SLOT(handleMGObjectRemoved(MapGraphicsObject*)));

}
