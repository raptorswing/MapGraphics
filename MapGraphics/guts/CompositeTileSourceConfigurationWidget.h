#ifndef COMPOSITETILESOURCECONFIGURATIONWIDGET_H
#define COMPOSITETILESOURCECONFIGURATIONWIDGET_H

#include <QWidget>
#include <QWeakPointer>
#include <QListView>

#include "MapGraphics_global.h"
#include "tileSources/CompositeTileSource.h"

namespace Ui {
class CompositeTileSourceConfigurationWidget;
}

class MAPGRAPHICSSHARED_EXPORT CompositeTileSourceConfigurationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CompositeTileSourceConfigurationWidget(QWeakPointer<CompositeTileSource> composite,QWidget *parent = 0);
    ~CompositeTileSourceConfigurationWidget();

private slots:
    void handleCurrentSelectionChanged(QModelIndex,QModelIndex);
    void handleCompositeChange();
    void on_removeSourceButton_clicked();

    void on_opacitySlider_valueChanged(int value);

    void on_moveDownButton_clicked();

    void on_moveUpButton_clicked();

private:
    Ui::CompositeTileSourceConfigurationWidget *ui;
    QWeakPointer<CompositeTileSource> _composite;
};

#endif // COMPOSITETILESOURCECONFIGURATIONWIDGET_H
