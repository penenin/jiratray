#ifndef WORKLOGDELEGATE_H
#define WORKLOGDELEGATE_H

#include <QItemDelegate>

class QModelIndex;
class QPainter;
class QStyleOptionViewItem;

class WorklogDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    const static int ID_COLUMN = 0;
    const static int ISSUE_COLUMN = 1;
    const static int DESCRIPTION_COLUMN = 2;
    const static int START_TIME_COLUMN = 3;
    const static int END_TIME_COLUMN = 4;
    const static int SENT_COLUMN = 5;

    WorklogDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

#endif // WORKLOGDELEGATE_H
