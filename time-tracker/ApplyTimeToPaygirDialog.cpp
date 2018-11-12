#include "ApplyTimeToPaygirDialog.h"
#include "ui_ApplyTimeToPaygirDialog.h"

ApplyTimeToPaygirDialog::ApplyTimeToPaygirDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ApplyTimeToPaygirDialog)
{
	ui->setupUi(this);
}

ApplyTimeToPaygirDialog::~ApplyTimeToPaygirDialog()
{
	delete ui;
}
