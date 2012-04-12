#ifndef PRIVATEQGRAPHICSSCENE_H
#define PRIVATEQGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QHash>
#include <QPointer>

#include "MapGraphicsScene.h"
#include "MapGraphicsObject.h"
#include "PrivateQGraphicsObject.h"

class PrivateQGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PrivateQGraphicsScene(MapGraphicsScene * mgScene,QObject *parent = 0);
    
signals:
    
public slots:

private slots:
    void handleMGObjectAdded(MapGraphicsObject *);
    void handleMGObjectRemoved(MapGraphicsObject *);

private:
    void setMapGraphicsScene(MapGraphicsScene * mgScene);

    QPointer<MapGraphicsScene> _mgScene;

    QHash<MapGraphicsObject *,PrivateQGraphicsObject *> _mgToqg;
    
};

#endif // PRIVATEQGRAPHICSSCENE_H
