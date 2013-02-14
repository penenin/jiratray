#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QByteArray;
class QString;

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

    static QString encryptPassword(const QString& password);
    static QString decryptPassword(const QString& encrypted);

    QString url() const;
    QString userName() const;
    QString password() const;
    bool savePassword() const;

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
