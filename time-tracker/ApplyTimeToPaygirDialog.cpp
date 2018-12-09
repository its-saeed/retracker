#include "ApplyTimeToPaygirDialog.h"
#include "ui_ApplyTimeToPaygirDialog.h"
#include "Settings.h"
#include "commons.h"
#include "DatabaseManager.h"

#include <QProcess>
#include <QDebug>
#include <QDate>

ApplyTimeToPaygirDialog::ApplyTimeToPaygirDialog(IssueManager& manager, QWidget *parent) :
QDialog(parent),
ui(new Ui::ApplyTimeToPaygirDialog)
, issue_manager(manager)
{
	ui->setupUi(this);
	update_issues();
}

ApplyTimeToPaygirDialog::~ApplyTimeToPaygirDialog()
{
	delete ui;
}

void ApplyTimeToPaygirDialog::update_issues()
{
	for (const auto& kv : issue_manager.get_issues())
	{
		const Issue& issue = kv.second;
		if (issue.get_total_duration(false) == std::chrono::seconds(0)
				/*|| issue.get_id() >= 1000000*/)		// id > 1000000 is for user defined issues
			continue;

		add_issue_to_table(issue);
	}
}

void ApplyTimeToPaygirDialog::add_issue_to_table(const Issue& issue)
{
	int row_count = ui->tblw_issues->rowCount();
	ui->tblw_issues->setRowCount(row_count + 1);

	// Issue ID Item in table
	auto issue_id_item = new QTableWidgetItem(QString::number(issue.get_id()));
	ui->tblw_issues->setItem(row_count, ISSUE_ID_COLUMN, issue_id_item);
	issue_id_item->setCheckState(Qt::Unchecked);
	issue_id_item->setFlags(issue_id_item->flags() & ~Qt::ItemIsEditable);

	auto issue_subject_item = new QTableWidgetItem(issue.get_subject());
	ui->tblw_issues->setItem(row_count, ISSUE_SUBJECT_COLUMN, issue_subject_item);
	issue_subject_item->setFlags(issue_subject_item->flags() & ~Qt::ItemIsEditable);

	auto issue_total_time_item = new QTableWidgetItem(issue.get_total_duration_string(false));
	ui->tblw_issues->setItem(row_count, ISSUE_TOTAL_TIME_COLUMN, issue_total_time_item);
	issue_total_time_item->setFlags(issue_total_time_item->flags() & ~Qt::ItemIsEditable);

	auto issue_total_applied_time_item = new QTableWidgetItem(issue.get_total_duration_string(false));
	ui->tblw_issues->setItem(row_count, ISSUE_PERFECT_TIME_COLUMN, issue_total_applied_time_item);

	auto comments_item = new QTableWidgetItem("Added by Retracker");
	ui->tblw_issues->setItem(row_count, ISSUE_COMMENTS_COLUMN, comments_item);
}

void ApplyTimeToPaygirDialog::on_btn_apply_times_clicked()
{
	ui->prg_apply->setRange(0, ui->tblw_issues->rowCount());
	for (int i = 0; i < ui->tblw_issues->rowCount(); ++i)
	{
		ui->prg_apply->setValue(i + 1);
		QTableWidgetItem* id_item = ui->tblw_issues->item(i, ISSUE_ID_COLUMN);
		if (id_item->checkState() == Qt::Unchecked)
			continue;

		const QString issue_id = ui->tblw_issues->item(i, ISSUE_ID_COLUMN)->text();
		const QString issue_total_time = ui->tblw_issues->item(i, ISSUE_TOTAL_TIME_COLUMN)->text();
		const QString issue_total_time_perfect = ui->tblw_issues->item(i, ISSUE_PERFECT_TIME_COLUMN)->text();
		const QString issue_comments = ui->tblw_issues->item(i, ISSUE_COMMENTS_COLUMN)->text();
		add_time_entry_to_peygir(issue_id, issue_total_time, issue_total_time_perfect, issue_comments);

		issue_manager.set_issue_timeslices_applied_to_redmine(issue_id.toInt());
		DatabaseManager::instance().set_issue_timeslices_applied_to_redmine(issue_id.toInt());
	}
}

void ApplyTimeToPaygirDialog::add_time_entry_to_peygir(const QString& issue_id, const QString& total_time,
													   const QString& total_perfect_time, const QString& comment)
{
	QString date = QLocale(QLocale::English).toString(QDate::currentDate(), "yyyy-MM-dd");
	const QString username = Settings::instance().get_username();
	const QString password = Settings::instance().get_password();

	QStringList arguments;
	arguments << "-u" << username << "-p" << password << "-i" << issue_id << "-d" << date
	<< "-sh" << QString::number(commons::time_to_double(total_time)) << "-ih" << QString::number(commons::time_to_double(total_perfect_time))
	<< "-c" << comment << "-a" << "9";

	QProcess process;
	process.start("scripts/create_time_entry.exe", arguments);
	process.waitForFinished();
	qDebug() << process.readAllStandardError();
	qDebug() << arguments;
}

void ApplyTimeToPaygirDialog::on_chb_select_all_toggled(bool checked)
{
	for (int i = 0; i < ui->tblw_issues->rowCount(); ++i)
		ui->tblw_issues->item(i, 0)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
}

void ApplyTimeToPaygirDialog::on_btn_today_clicked()
{

}
