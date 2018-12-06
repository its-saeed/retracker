#include "AddIssueDialog.h"
#include "ui_AddIssueDialog.h"
#include "DatabaseManager.h"

#include <QRandomGenerator>

AddIssueDialog::AddIssueDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddIssueDialog)
{
	ui->setupUi(this);
	load_categories();
}

AddIssueDialog::~AddIssueDialog()
{
	delete ui;
}

bool AddIssueDialog::load_from_redmine() const
{
	return ui->rdb_load_from_redmine->isChecked();
}

bool AddIssueDialog::create_new_issue() const
{
	return ui->rdb_new_issue->isChecked();
}

int AddIssueDialog::issue_id() const
{
	QRandomGenerator gen(QTime::currentTime().second());

	if (load_from_redmine())
		return ui->led_issue_id->text().toInt();
	return 1000000 + gen.generate();
}

QString AddIssueDialog::get_subject() const
{
	return ui->led_subject->text();
}

int AddIssueDialog::get_state() const
{
	return ui->cmb_category->currentData().toInt();
}

void AddIssueDialog::load_categories()
{
	auto cats = DatabaseManager::instance().all_categories();

	for (auto& cat : cats)
		ui->cmb_category->addItem(cat.second, cat.first);
}
