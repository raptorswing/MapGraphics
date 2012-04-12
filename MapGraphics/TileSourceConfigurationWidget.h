#ifndef TILESOURCECONFIGURATIONWIDGET_H
#define TILESOURCECONFIGURATIONWIDGET_H

#include <QWidget>
#include <QWeakPointer>

#include "MapGraphics_global.h"
#include "CompositeTileSource.h"

namespace Ui {
class TileSourceConfigurationWidget;
}

class MAPGRAPHICSSHARED_EXPORT TileSourceConfigurationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit TileSourceConfigurationWidget(QWeakPointer<CompositeTileSource> composite,QWidget *parent = 0);
    ~TileSourceConfigurationWidget();
    
private:
    Ui::TileSourceConfigurationWidget *ui;
    QWeakPointer<CompositeTileSource> _composite;
};

#endif // TILESOURCECONFIGURATIONWIDGET_H
