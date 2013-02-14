#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCalendarWidget>
#include <QCloseEvent>
#include <QDateTime>
#include <QMessageBox>
#include <QMenu>
#include <QProgressDialog>
#include <QSettings>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include "editworklog.h"
#include "jira.h"
#include "settingsdialog.h"

#include "worklogdelegate.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <Security/Security.h>
#else
#include "simplecrypt.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();
    createModel();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editWorklog(QModelIndex)));
    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(refreshTableModel()));
    connect(ui->actionAddNewWorklog, SIGNAL(triggered()), this, SLOT(addNewWorklog()));
    connect(ui->actionSubmitToJIRA, SIGNAL(triggered()), this, SLOT(submitToJIRA()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    setWindowTitle(tr("JIRA Tray"));

    hide();
    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    showAction = new QAction(tr("Show"), this);
    connect(showAction, SIGNAL(triggered()), this, SLOT(show()));
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(showAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui->actionQuit);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    QIcon icon(":/myresources/stopwatch.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void MainWindow::createModel()
{
    tableModel = new QSqlTableModel(this);
    tableModel->setTable("worklog");
    tableModel->setHeaderData(WorklogDelegate::ISSUE_COLUMN, Qt::Horizontal, tr("Issue"));
    tableModel->setHeaderData(WorklogDelegate::DESCRIPTION_COLUMN, Qt::Horizontal, tr("Description"));
    tableModel->setHeaderData(WorklogDelegate::START_TIME_COLUMN, Qt::Horizontal, tr("Start"));
    tableModel->setHeaderData(WorklogDelegate::END_TIME_COLUMN, Qt::Horizontal, tr("End"));
    tableModel->setHeaderData(WorklogDelegate::SENT_COLUMN, Qt::Horizontal, tr("Sent"));
    tableModel->setSort(WorklogDelegate::START_TIME_COLUMN, Qt::AscendingOrder);
    refreshTableModel();

    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegate(new WorklogDelegate());
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setColumnHidden(WorklogDelegate::ID_COLUMN, true);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible())
    {
        QMessageBox::information(this, tr("Systray"),
                                 tr("The program will keep running in the "
                                    "system tray. To terminate the program, "
                                    "choose <b>Quit</b> in the context menu "
                                    "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        show();
        break;
    default:
        ;
    }
}

void MainWindow::addNewWorklog()
{
    EditWorklog dialog;
    dialog.setDate(ui->calendarWidget->selectedDate());
    if(dialog.exec() == QDialog::Accepted)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO worklog VALUES (NULL, :issue, :description, :start_time, :end_time, :sent)");
        query.bindValue(":issue", dialog.issue());
        query.bindValue(":description", dialog.description());
        query.bindValue(":start_time", dialog.startTime());
        query.bindValue(":end_time", dialog.endTime());
        query.bindValue(":sent", dialog.sent());
        query.exec();
        refreshTableModel();
    }
}

void MainWindow::editWorklog(const QModelIndex &index)
{
    EditWorklog dialog;
    if(index.isValid())
    {
        dialog.setId(tableModel->record(index.row()).value("id").toInt());
    }
    if(dialog.exec() == QDialog::Accepted)
    {
        QSqlQuery query;
        query.prepare("UPDATE worklog SET issue = :issue, description = :description, "
                      "start_time = :start_time, end_time = :end_time, sent = :sent "
                      "WHERE id = :id");
        query.bindValue(":issue", dialog.issue());
        query.bindValue(":description", dialog.description());
        query.bindValue(":start_time", dialog.startTime());
        query.bindValue(":end_time", dialog.endTime());
        query.bindValue(":sent", dialog.sent());
        query.bindValue(":id", dialog.id());
        query.exec();
        refreshTableModel();
    }
}

void MainWindow::refreshTableModel()
{
    tableModel->setFilter(QString("start_time >= '%1' and start_time < '%2'")
                          .arg(ui->calendarWidget->selectedDate().toString(Qt::ISODate))
                          .arg(ui->calendarWidget->selectedDate().addDays(1).toString(Qt::ISODate)));
    tableModel->select();
}

void MainWindow::submitToJIRA()
{
    try
    {
        tableModel->setFilter(QString("sent = 'false'"));
        tableModel->select();

        QProgressDialog progress("Submitting Worklogs to JIRA", "Cancel", 0, tableModel->rowCount(), this);
        progress.setWindowModality(Qt::WindowModal);

        QSettings settings;
        std::string userName = settings.value("userName").toString().toStdString();

#ifdef __APPLE__
        UInt32 storedPasswordLength;
        char *storedPassword;
        std::string password;

        OSStatus result = SecKeychainFindGenericPassword(NULL,
                                                         9,
                                                         "JIRA Tray",
                                                         userName.length(),
                                                         userName.c_str(),
                                                         &storedPasswordLength,
                                                         reinterpret_cast<void **>(&storedPassword),
                                                         NULL);
        if(result == noErr)
        {
            password = storedPassword;
            SecKeychainItemFreeContent(NULL, storedPassword);
        }
        else
        {
            QMessageBox::information(this, tr("Keychain Error"), tr("A keychain error occurred: %1").arg(result));
            return;
        }
#else
        // TOOD: change seed to hash of username
        SimpleCrypt crypto(Q_UINT64_C(0xed55a41b90f0b66d));
        std::string password = crypto.decryptToString(settings.value("password").toString()).toStdString();
#endif

        JIRA soap(settings.value("url").toString().toAscii().constData());
        std::string token;
        soap.login(userName, password, token);

        QSqlQuery query;
        query.prepare("UPDATE worklog SET sent = 'true' WHERE id = :id");

        for(int i = 0; i < tableModel->rowCount(); i++)
        {
            progress.setValue(i);
            if (progress.wasCanceled())
            {
                break;
            }

            QSqlRecord record = tableModel->record(i);

            JIRA::RemoteWorklog worklog;
            JIRA::RemoteWorklog worklogReturn;
            worklog.author = userName;
            worklog.comment = record.value(WorklogDelegate::DESCRIPTION_COLUMN).toString().toStdString();
            worklog.created = "";
            worklog.groupLevel = "";
            worklog.id = "";
            worklog.roleLevelId = "";
            QDateTime startTime = record.value(WorklogDelegate::START_TIME_COLUMN).toDateTime().toUTC();
            worklog.startDate = startTime.toString(Qt::ISODate).toStdString();
            QDateTime endTime = record.value(WorklogDelegate::END_TIME_COLUMN).toDateTime().toUTC();
            worklog.timeSpent = calculateJIRATimeSpent(startTime, endTime).toStdString();
            worklog.timeSpentInSeconds = 0;
            worklog.updateAuthor = "";
            worklog.updated = "";
            soap.addWorklogAndAutoAdjustRemainingEstimate(token,
                                                          record.value(WorklogDelegate::ISSUE_COLUMN).toString().toStdString(),
                                                          worklog,
                                                          worklogReturn);

            query.bindValue(":id", record.value(WorklogDelegate::ID_COLUMN).toInt());
            query.exec();
        }

        bool logoutReturn;
        soap.logout(token, logoutReturn);

        progress.setValue(tableModel->rowCount());
        refreshTableModel();

        QMessageBox::information(this, tr("Success"), tr("Worklogs successfully sent to JIRA."));
    }
    catch (SOAPException& ex)
    {
        QMessageBox::information(this, tr("SOAP Error"), ex.What().Str());
    }
}

QString MainWindow::calculateJIRATimeSpent(QDateTime startTime, QDateTime endTime, bool roundToNearest15Minutes)
{
    int secs = startTime.secsTo(endTime);
    int mins = secs / 60;

    // Round to nearest minute
    secs %= 60;
    if(secs >= 30)
    {
        mins++;
    }

    if(roundToNearest15Minutes)
    {
        int mod = mins % 15;
        if(mod >= 8)
        {
            mins += (15 - mod);
        }
        else
        {
            mins -= mod;
        }
    }

    int hours = mins / 60;
    int days = hours / 8;


    mins %= 60;
    hours %= 8;

    QString ret;
    bool needSpace = false;

    if(days > 0)
    {
        ret.append(QString("%1d").arg(days));
        needSpace = true;
    }

    if(hours > 0)
    {
        if(needSpace)
        {
            ret.append(' ');
        }
        ret.append(QString("%1h").arg(hours));
        needSpace = true;
    }

    if(mins > 0)
    {
        if(needSpace)
        {
            ret.append(' ');
        }
        ret.append(QString("%1m").arg(mins));
    }

    return ret;
}

void MainWindow::settings()
{
    SettingsDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        QSettings settings;
        settings.setValue("url", dialog.url());
        settings.setValue("userName", dialog.userName());
        if(dialog.savePassword())
        {
#ifdef __APPLE__
            SecKeychainItemRef itemRef = NULL;
            UInt32 storedPasswordLength;
            char *storedPassword = NULL;
            std::string password;

            OSStatus result = SecKeychainFindGenericPassword(NULL,
                                                             9,
                                                             "JIRA Tray",
                                                             dialog.userName().length(),
															 dialog.userName().toStdString().c_str(),
                                                             &storedPasswordLength,
                                                             reinterpret_cast<void **>(&storedPassword),
                                                             &itemRef);
            if(result == errSecItemNotFound)
            {
                if(SecKeychainAddGenericPassword(NULL,
                                                 9,
                                                 "JIRA Tray",
                                                 dialog.userName().length(),
                                                 dialog.userName().toStdString().c_str(),
                                                 dialog.password().length(),
                                                 dialog.password().toStdString().c_str(),
                                                 NULL) != noErr)
                {
                    QMessageBox::information(this, tr("Keychain Error"), tr("A keychain error occurred: %1").arg(result));
                }
            }
            else if(result == noErr)
            {
                if(SecKeychainItemModifyAttributesAndData(itemRef,
                                                          NULL,
                                                          dialog.password().length(),
                                                          dialog.password().toStdString().c_str()) != noErr)
                {
                    QMessageBox::information(this, tr("Keychain Error"), tr("A keychain error occurred: %1").arg(result));
                }

            }
            else
            {
                QMessageBox::information(this, tr("Keychain Error"), tr("A keychain error occurred: %1").arg(result));
            }

            if(storedPassword)
            {
                SecKeychainItemFreeContent(NULL, storedPassword);
            }

            if(itemRef)
            {
                CFRelease(itemRef);
            }
#else
            // TOOD: change seed to hash of username
            SimpleCrypt crypto(Q_UINT64_C(0xed55a41b90f0b66d));
            settings.setValue("password", crypto.encryptToString(dialog.password()));
#endif
        }
    }
}
