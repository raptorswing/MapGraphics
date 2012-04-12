#include "TileSourceConfigurationWidget.h"
#include "ui_TileSourceConfigurationWidget.h"

#include "MapTileLayerListModel.h"

TileSourceConfigurationWidget::TileSourceConfigurationWidget(QWeakPointer<CompositeTileSource> composite,
                                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TileSourceConfigurationWidget),
    _composite(composite)
{
    ui->setupUi(this);

    MapTileLayerListModel * model = new MapTileLayerListModel(_composite,this);
    this->ui->listView->setModel(model);
}

TileSourceConfigurationWidget::~TileSourceConfigurationWidget()
{
    delete ui;
}
