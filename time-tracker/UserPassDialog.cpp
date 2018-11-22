#include "UserPassDialog.h"
#include "ui_UserPassDialog.h"
#include "Settings.h"

UserPassDialog::UserPassDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserPassDialog)
{
    ui->setupUi(this);
	ui->chb_remember->setChecked(Settings::instance().remember_userpass());
	ui->led_user->setText(Settings::instance().get_username());
	ui->led_pass->setText(Settings::instance().get_password());
}

UserPassDialog::~UserPassDialog()
{
    delete ui;
}

QString UserPassDialog::get_user_name() const
{
    return ui->led_user->text();
}

QString UserPassDialog::get_password() const
{
    return ui->led_pass->text();
}

void UserPassDialog::on_btn_ok_clicked()
{
	Settings::instance().save_username(ui->chb_remember->isChecked() ? ui->led_user->text() : "");
	Settings::instance().save_password(ui->chb_remember->isChecked() ? ui->led_pass->text() : "");
	Settings::instance().set_remember_userpass(ui->chb_remember->isChecked());
	accept();
}

Credential UserPassDialog::get_credential()
{
	QString username = Settings::instance().get_username();
	QString password = Settings::instance().get_password();

	if (username.isEmpty() || password.isEmpty())
	{
		UserPassDialog dlg;
		if (dlg.exec() != QDialog::Rejected)
		{
			username = dlg.get_user_name();
			password = dlg.get_password();
		}
	}

	return {username, password};
}
