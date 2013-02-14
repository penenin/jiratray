#include "worklogdelegate.h"

#include <QDateTime>
#include <QModelIndex>
#include <QPainter>
#include <QString>
#include <QStyleOptionViewItem>

WorklogDelegate::WorklogDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

void WorklogDelegate::paint(QPainter *painter,
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index) const
{
    int column = index.column();
    if (column == START_TIME_COLUMN || column == END_TIME_COLUMN)
    {
        drawDisplay(painter, option, option.rect,
                    index.model()->data(index).toDateTime().toString("HH:mm"));
        drawFocus(painter, option, option.rect);
    }
    else if(column == SENT_COLUMN)
    {
        drawCheck(painter, option, option.rect,
                  index.model()->data(index).toBool() ? Qt::Checked : Qt::Unchecked);
        drawFocus(painter, option, option.rect);
    }
    else
    {
        QItemDelegate::paint(painter, option, index);
    }
}
