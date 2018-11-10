#ifndef TIMEPICKERDIALOG_H
#define TIMEPICKERDIALOG_H

#include <QDialog>

namespace Ui {
class TimePickerDialog;
}

class TimePickerDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TimePickerDialog(QWidget *parent = 0);
	~TimePickerDialog();
	QTime get_time() const;

private slots:
	void on_btn_ok_clicked();

private:
	Ui::TimePickerDialog *ui;
};

#endif // TIMEPICKERDIALOG_H
