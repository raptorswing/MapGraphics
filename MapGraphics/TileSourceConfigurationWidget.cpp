#include "TileSourceConfigurationWidget.h"
#include "ui_TileSourceConfigurationWidget.h"

#include "MapTileLayerListModel.h"
#include "MapTileSourceDelegate.h"

TileSourceConfigurationWidget::TileSourceConfigurationWidget(QWeakPointer<CompositeTileSource> composite,
                                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TileSourceConfigurationWidget),
    _composite(composite)
{
    ui->setupUi(this);

    MapTileSourceDelegate * delegato = new MapTileSourceDelegate(this);
    MapTileLayerListModel * model = new MapTileLayerListModel(_composite,this);
    this->ui->listView->setModel(model);
    this->ui->listView->setItemDelegate(delegato);
}

TileSourceConfigurationWidget::~TileSourceConfigurationWidget()
{
    delete ui;
}
