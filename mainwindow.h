#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class QCalendarWidget;
class QCloseEvent;
class QDateTime;
class QMenu;
class QModelIndex;
class QSqlTableModel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
     void closeEvent(QCloseEvent *event);

private slots:
     void iconActivated(QSystemTrayIcon::ActivationReason reason);
     void addNewWorklog();
     void editWorklog(const QModelIndex &index);
     void refreshTableModel();
     void submitToJIRA();
     void settings();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *showAction;
    QSqlTableModel *tableModel;

    static QString calculateJIRATimeSpent(QDateTime startTime, QDateTime endTime, bool roundToNearest15Minutes = true);

    void createActions();
    void createTrayIcon();
    void createModel();
};

#endif // MAINWINDOW_H
