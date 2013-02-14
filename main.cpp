#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QDir>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

bool createSchema()
{
    QSqlQuery query;
    query.exec("CREATE TABLE worklog("
               "id INTEGER PRIMARY KEY,"
               "issue TEXT,"
               "description TEXT,"
               "start_time TEXT,"
               "end_time TEXT,"
               "sent TEXT"
               ")");
    return true;
}

bool checkSchema()
{
    QSqlQuery query;
    query.exec("SELECT count(*) FROM sqlite_master WHERE type = 'table'");
    while (query.next())
    {
        int count = query.value(0).toInt();
        if(count == 0)
        {
            return createSchema();
        }
    }
    return true;
}

bool createConnection()
{
    QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    QDir::setCurrent(QDir::homePath());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("jiratray.sqlite3");
    if (!db.open())
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),
                              db.lastError().text());
        return false;
    }
    return checkSchema();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection())
        return 1;

    QCoreApplication::setOrganizationName("unmei.org");
    QCoreApplication::setOrganizationDomain("unmei.org");
    QCoreApplication::setApplicationName("Jiratray");

    MainWindow w;
    w.show();

    return a.exec();
}
