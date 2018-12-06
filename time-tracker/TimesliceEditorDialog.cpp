#include "TimesliceEditorDialog.h"
#include "ui_TimesliceEditorDialog.h"

TimesliceEditorDialog::TimesliceEditorDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TimesliceEditorDialog)
{
	ui->setupUi(this);
}

TimesliceEditorDialog::~TimesliceEditorDialog()
{
	delete ui;
}
