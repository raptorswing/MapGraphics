#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MapGraphicsView.h"
#include "guts/MapGraphicsScene.h"
#include "guts/MapInfoManager.h"
#include "guts/MapQuestTileSource.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*
      Create a tile source that MapGraphics will use to fetch tiles.
      In this case, we'll be using MapQuest's OSM tiles.
    */
    QSharedPointer<MapTileSource> tileSource(new MapQuestTileSource(MapQuestTileSource::OSM_TILES));

    //Tell MapGraphics to use the tile source we've just created
    MapInfoManager::getInstance()->setMapTileSource(tileSource);

    /*
      Create a MapGraphics scene.
      Note that currently only one view can use a scene. Lame, I know.
    */
    MapGraphicsScene * scene = new MapGraphicsScene(this);

    //Create the actual map widget, a MapGraphicsView
    MapGraphicsView * view = new MapGraphicsView(this);

    //Tell our view to use the scene!
    view->setScene(scene);

    //Set the view as our central widget
    this->setCentralWidget(view);

    //Configure the view to allow panning the map by dragging the mouse
    view->setDragMode(QGraphicsView::ScrollHandDrag);
}

MainWindow::~MainWindow()
{
    delete ui;
}
