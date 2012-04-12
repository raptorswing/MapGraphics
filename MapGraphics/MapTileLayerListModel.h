#ifndef MAPTILELAYERLISTMODEL_H
#define MAPTILELAYERLISTMODEL_H

#include <QAbstractListModel>
#include <QWeakPointer>

#include "CompositeTileSource.h"

class MapTileLayerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MapTileLayerListModel(QWeakPointer<CompositeTileSource> composite,QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    
signals:
    
public slots:

private slots:
    void handleCompositeSourcesChanged();

private:
    QWeakPointer<CompositeTileSource> _composite;
    
};

#endif // MAPTILELAYERLISTMODEL_H
