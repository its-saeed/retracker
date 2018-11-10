#include "TimePickerDialog.h"
#include "ui_TimePickerDialog.h"

TimePickerDialog::TimePickerDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::TimePickerDialog)
{
	ui->setupUi(this);
	ui->te_today->setTime(QTime::currentTime());
}

TimePickerDialog::~TimePickerDialog()
{
	delete ui;
}

QTime TimePickerDialog::get_time() const
{
	return ui->te_today->time();
}

void TimePickerDialog::on_btn_ok_clicked()
{
	accept();
}
