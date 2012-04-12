#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "MapGraphicsView.h"
#include "MapGraphicsScene.h"
#include "GridTileSource.h"
#include "OSMTileSource.h"
#include "CompositeTileSource.h"

#include <QSharedPointer>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MapGraphicsScene * scene = new MapGraphicsScene(this);
    MapGraphicsView * view = new MapGraphicsView(scene,this);

    this->setCentralWidget(view);

    QSharedPointer<OSMTileSource> osmTiles(new OSMTileSource(OSMTileSource::OSMTiles));
    QSharedPointer<OSMTileSource> aerialTiles(new OSMTileSource(OSMTileSource::MapQuestAerialTiles));
    QSharedPointer<GridTileSource> gridTiles(new GridTileSource());
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource());
    composite->addSourceBottom(osmTiles,0.75);
    composite->addSourceBottom(aerialTiles);
    composite->addSourceTop(gridTiles);
    view->setTileSource(composite);
    qDebug() << "init finished";
}

MainWindow::~MainWindow()
{
    delete ui;
}
