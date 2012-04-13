#include "MapTileSourceDelegate.h"

#include <QPainter>
#include <QtDebug>

MapTileSourceDelegate::MapTileSourceDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

//virtual from QStyledItemDelegate
void MapTileSourceDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();

    //Get the palette that we should use
    QPalette palette = option.palette;

    //Use a slightly smaller rect so that we can see our border better
    QRect rect = option.rect;
    rect.setWidth(rect.width()-2);

    //Based on palette and state, choose background color
    QBrush backgroundBrush = palette.base();
    QColor borderColor = palette.text().color();
    QColor textColor = palette.text().color();
    if (option.state & QStyle::State_Selected)
        backgroundBrush = palette.highlight();

    //Draw background
    painter->fillRect(rect,backgroundBrush);

    //Draw border
    painter->setPen(borderColor);
    painter->drawRect(rect);

    //Draw text
    QRect textRect = rect;
    textRect.adjust(1,0,-1,0);
    QString string = index.model()->data(index).toString();
    painter->setPen(textColor);
    painter->drawText(textRect,string,QTextOption(Qt::AlignLeft | Qt::AlignVCenter));

    painter->restore();
}

QSize MapTileSourceDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    QSize toRet;
    toRet.setWidth(250);
    toRet.setHeight(25);

    return toRet;
}
