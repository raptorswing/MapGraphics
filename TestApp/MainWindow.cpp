#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MapGraphicsView.h"
#include "MapGraphicsScene.h"
#include "GridTileSource.h"
#include "OSMTileSource.h"
#include "CompositeTileSource.h"
#include "TileSourceConfigurationWidget.h"

#include <QSharedPointer>
#include <QtDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup the MapGraphics scene and view
    MapGraphicsScene * scene = new MapGraphicsScene(this);
    MapGraphicsView * view = new MapGraphicsView(scene,this);

    //The view will be our central widget
    this->setCentralWidget(view);

    //Setup some tile sources
    QSharedPointer<OSMTileSource> osmTiles(new OSMTileSource(OSMTileSource::OSMTiles));
    QSharedPointer<OSMTileSource> aerialTiles(new OSMTileSource(OSMTileSource::MapQuestAerialTiles));
    QSharedPointer<GridTileSource> gridTiles(new GridTileSource());
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource());
    composite->addSourceBottom(osmTiles,0.75);
    composite->addSourceBottom(aerialTiles);
    composite->addSourceTop(gridTiles);
    view->setTileSource(composite);

    //Throw the tile sources in a different thread
    QThread * tileThread = new QThread(this);
    composite->moveToThread(tileThread);
    gridTiles->moveToThread(tileThread);
    aerialTiles->moveToThread(tileThread);
    osmTiles->moveToThread(tileThread);
    tileThread->start();

    //Make sure threads are shutdown when it's time to quit
    connect(this,
            SIGNAL(destroyed()),
            tileThread,
            SLOT(quit()));

    TileSourceConfigurationWidget * tileConfigWidget = new TileSourceConfigurationWidget(composite.toWeakRef(),
                                                                                         this->ui->dockWidget);
    this->ui->dockWidget->setWidget(tileConfigWidget);
    delete this->ui->dockWidgetContents;


}

MainWindow::~MainWindow()
{
    delete ui;
}
