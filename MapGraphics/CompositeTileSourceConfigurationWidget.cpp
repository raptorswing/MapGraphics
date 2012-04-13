#include "CompositeTileSourceConfigurationWidget.h"
#include "ui_CompositeTileSourceConfigurationWidget.h"

#include "MapTileLayerListModel.h"
#include "MapTileSourceDelegate.h"

#include <QtDebug>

CompositeTileSourceConfigurationWidget::CompositeTileSourceConfigurationWidget(QWeakPointer<CompositeTileSource> composite,
                                                             QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompositeTileSourceConfigurationWidget),
    _composite(composite)
{
    ui->setupUi(this);

    //Create a fancy delegate for custom drawing of our list items
    MapTileSourceDelegate * delegato = new MapTileSourceDelegate(composite,this);

    //Create the model that watches the CompositeTileSource
    MapTileLayerListModel * model = new MapTileLayerListModel(_composite,this);

    //Set the QListView to watch the model and use the delegate
    this->ui->listView->setModel(model);
    this->ui->listView->setItemDelegate(delegato);
}

CompositeTileSourceConfigurationWidget::~CompositeTileSourceConfigurationWidget()
{
    delete ui;
}


void CompositeTileSourceConfigurationWidget::on_removeSourceButton_clicked()
{
    QItemSelectionModel * selModel = this->ui->listView->selectionModel();
    QModelIndexList selections = selModel->selectedIndexes();

    QSharedPointer<CompositeTileSource> strong = _composite.toStrongRef();
    if (strong.isNull())
        return;

    foreach (QModelIndex index, selections)
    {
        strong->removeSource(index.row());
    }
}
