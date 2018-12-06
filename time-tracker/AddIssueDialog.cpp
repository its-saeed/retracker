#include "AddIssueDialog.h"
#include "ui_AddIssueDialog.h"

AddIssueDialog::AddIssueDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddIssueDialog)
{
	ui->setupUi(this);
}

AddIssueDialog::~AddIssueDialog()
{
	delete ui;
}
