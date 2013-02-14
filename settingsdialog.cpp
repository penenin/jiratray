#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QByteArray>
#include <QSettings>
#include <QString>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->savePasswordCheckBox, SIGNAL(toggled(bool)), ui->passwordLine, SLOT(setEnabled(bool)));
    //connect(ui->savePasswordCheckBox, SIGNAL(toggled(bool)), ui->passwordLine, SLOT(clear()));

    QSettings settings;
    ui->urlLine->setText(settings.value("url", QString()).toString());
    ui->userNameLine->setText(settings.value("userName", QString()).toString());
    QString password = settings.value("password", QString()).toString();
    if(!password.isEmpty())
    {
        ui->passwordLine->setText(password);
        ui->savePasswordCheckBox->setChecked(true);
    }

    setWindowTitle(tr("Settings"));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

QString SettingsDialog::url() const
{
    return ui->urlLine->text();
}

QString SettingsDialog::userName() const
{
    return ui->userNameLine->text();
}

QString SettingsDialog::password() const
{
    return ui->passwordLine->text();
}

bool SettingsDialog::savePassword() const
{
    return ui->savePasswordCheckBox->isChecked();
}
