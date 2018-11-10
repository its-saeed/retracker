#ifndef USERPASSDIALOG_H
#define USERPASSDIALOG_H

#include <QDialog>

namespace Ui {
class UserPassDialog;
}

class UserPassDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserPassDialog(QWidget *parent = 0);
    ~UserPassDialog();
    QString get_user_name() const;
    QString get_password() const;

private slots:
	void on_btn_ok_clicked();

private:
    Ui::UserPassDialog *ui;
};

#endif // USERPASSDIALOG_H
