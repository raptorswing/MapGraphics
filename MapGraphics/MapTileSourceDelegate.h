#ifndef MAPTILESOURCEDELEGATE_H
#define MAPTILESOURCEDELEGATE_H

#include <QStyledItemDelegate>

class MapTileSourceDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MapTileSourceDelegate(QObject *parent = 0);

    //virtual from QStyledItemDelegate
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // MAPTILESOURCEDELEGATE_H
