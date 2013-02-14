#ifndef EDITWORKLOG_H
#define EDITWORKLOG_H

#include <QDialog>

class QDate;
class QDateTime;

namespace Ui {
    class EditWorklog;
}

class EditWorklog : public QDialog
{
    Q_OBJECT

public:
    explicit EditWorklog(QWidget *parent = 0);
    ~EditWorklog();

    int id() const;
    void setId(const int id);
    QString issue() const;
    QString description() const;
    QDateTime startTime() const;
    QDateTime endTime() const;
    bool sent() const;
    void setDate(const QDate &date);

private:
    Ui::EditWorklog *ui;
    int _id;
};

#endif // EDITWORKLOG_H
