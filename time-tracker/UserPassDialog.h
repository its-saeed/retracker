#ifndef USERPASSDIALOG_H
#define USERPASSDIALOG_H

#include <QDialog>

namespace Ui {
class UserPassDialog;
}

struct Credential
{
	QString username;
	QString password;
};

class UserPassDialog : public QDialog
{
    Q_OBJECT

public:
	explicit UserPassDialog(QWidget *parent = nullptr);
    ~UserPassDialog();
    QString get_user_name() const;
    QString get_password() const;
	static Credential get_credential();
private slots:
	void on_btn_ok_clicked();

private:
    Ui::UserPassDialog *ui;
};

#endif // USERPASSDIALOG_H
