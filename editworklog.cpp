#include "editworklog.h"
#include "ui_editworklog.h"

#include <QDateTime>
#include <QSqlQuery>

#include "worklogdelegate.h"

EditWorklog::EditWorklog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWorklog),
    _id(0)
{
    ui->setupUi(this);

    setWindowTitle(tr("Edit Worklog"));
}

EditWorklog::~EditWorklog()
{
    delete ui;
}

int EditWorklog::id() const
{
    return _id;
}

void EditWorklog::setId(const int id)
{
    if(id > 0)
    {
        _id = id;
        QSqlQuery query;
        query.prepare("SELECT * FROM worklog WHERE id = :id");
        query.bindValue(":id", _id);
        query.exec();
        if(query.next())
        {
            ui->issueLine->setText(query.value(WorklogDelegate::ISSUE_COLUMN).toString());
            ui->descriptionText->setText(query.value(WorklogDelegate::DESCRIPTION_COLUMN).toString());
            ui->startTimeEdit->setDateTime(query.value(WorklogDelegate::START_TIME_COLUMN).toDateTime());
            ui->endTimeEdit->setDateTime(query.value(WorklogDelegate::END_TIME_COLUMN).toDateTime());
            ui->sentCheckBox->setChecked(query.value(WorklogDelegate::SENT_COLUMN).toBool());
        }
    }
}

QString EditWorklog::issue() const
{
    return ui->issueLine->text();
}

QString EditWorklog::description() const
{
    return ui->descriptionText->toPlainText();
}

QDateTime EditWorklog::startTime() const
{
    return ui->startTimeEdit->dateTime();
}

QDateTime EditWorklog::endTime() const
{
    return ui->endTimeEdit->dateTime();
}

bool EditWorklog::sent() const
{
    return ui->sentCheckBox->isChecked();
}

void EditWorklog::setDate(const QDate &date)
{
    ui->startTimeEdit->setDate(date);
    ui->endTimeEdit->setDate(date);
}
