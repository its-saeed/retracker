#include "TimesliceEditorDialog.h"
#include "ui_TimesliceEditorDialog.h"

#include <QDate>
#include <QMessageBox>
#include <QTime>

#include "DatabaseManager.h"
#include "commons.h"

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

void TimesliceEditorDialog::set_issue_manager(IssueManager* iss_m)
{
	issue_manager = iss_m;
}

void TimesliceEditorDialog::update_for_issue(Issue::Id id)
{
	current_issue_id = id;
	const Issue& issue = issue_manager->get_issue_by_id(id);

	int row = 0;
	for (auto& kv : issue.get_timeslices())
	{
		int col = 0;
		ui->tblw_timeslices->setRowCount(row + 1);
		const Timeslice& timeslice = kv.second;
		auto item = new QTableWidgetItem(QString::number(timeslice.id));
		ui->tblw_timeslices->setItem(row, col++, item);

		item = new QTableWidgetItem(timeslice.start.date().toString());
		ui->tblw_timeslices->setItem(row, col++, item);

		item = new QTableWidgetItem(timeslice.start.time().toString());
		ui->tblw_timeslices->setItem(row, col++, item);

		item = new QTableWidgetItem(timeslice.end.date().toString());
		ui->tblw_timeslices->setItem(row, col++, item);

		item = new QTableWidgetItem(timeslice.end.time().toString());
		ui->tblw_timeslices->setItem(row, col++, item);

		item = new QTableWidgetItem(timeslice.applied_to_redmine ? "Yes" : "No");
		ui->tblw_timeslices->setItem(row, col++, item);
		row++;
	}
}

void TimesliceEditorDialog::on_tblw_timeslices_currentCellChanged(int currentRow,
																  int, int, int)
{
	QDateTime start_date_time(QDate::fromString(ui->tblw_timeslices->item(currentRow, 1)->text()),
							  QTime::fromString(ui->tblw_timeslices->item(currentRow, 2)->text()));

	QDateTime end_date_time(QDate::fromString(ui->tblw_timeslices->item(currentRow, 3)->text()),
							  QTime::fromString(ui->tblw_timeslices->item(currentRow, 4)->text()));

	ui->dte_start->setDateTime(start_date_time);
	ui->dte_end->setDateTime(end_date_time);

	update_duration_label(start_date_time.time(), end_date_time.time());
}

void TimesliceEditorDialog::on_btn_apply_clicked()
{
	if (get_current_timeslice_id() == 0)
		return;

	const QDateTime& start = ui->dte_start->dateTime();
	const QDateTime& end = ui->dte_end->dateTime();

	if (start > end)
	{
		QMessageBox::warning(this, "Apply", "Start is ahead of the end !");
		return;
	}

	if (start.daysTo(end) != 0)
	{
		QMessageBox::warning(this, "Apply", "Start and end date should be in the same date!");
		return;
	}

	Timeslice ts{get_current_timeslice_id(), ui->dte_start->dateTime(), ui->dte_end->dateTime(), get_current_timeslice_applied_to_redmine()};
	if (!issue_manager->update_timeslice(current_issue_id, ts) || !DatabaseManager::instance().update_timeslice(ts))
	{
		QMessageBox::warning(this, "Error", "Couldn't update the timeslice");
		return;
	}

	ui->btn_apply->setText("Applied...");
	ui->btn_apply->setEnabled(false);

	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 1)->setText(start.date().toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 2)->setText(start.time().toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 3)->setText(end.date().toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 4)->setText(end.time().toString());
}

int TimesliceEditorDialog::get_current_timeslice_id() const
{
	auto item = ui->tblw_timeslices->currentItem();
	if (!item)
		return 0;

	return ui->tblw_timeslices->item(item->row(), 0)->text().toInt();
}

bool TimesliceEditorDialog::get_current_timeslice_applied_to_redmine() const
{
	auto item = ui->tblw_timeslices->currentItem();
	if (!item)
		return false;

	return (ui->tblw_timeslices->item(item->row(), 5)->text() == "Yes") ? true : false;
}

void TimesliceEditorDialog::update_duration_label(const QTime& start, const QTime& end)
{
	std::chrono::seconds seconds{start.secsTo(end)};

	const QString duration = QString::fromStdString(commons::to_string(seconds));
	ui->lbl_duration->setText(duration);

	ui->lbl_total_duration->setText(issue_manager->get_issue_by_id(current_issue_id).get_total_duration_string());
}

void TimesliceEditorDialog::on_dte_start_dateTimeChanged(const QDateTime &dateTime)
{
	const QTime start_time = dateTime.time();
	const QTime end_time = ui->dte_end->dateTime().time();
	update_duration_label(start_time, end_time);
	ui->btn_apply->setEnabled(true);
	ui->btn_apply->setText("Apply");
}

void TimesliceEditorDialog::on_dte_end_dateTimeChanged(const QDateTime& dateTime)
{
	const QTime start_time = ui->dte_start->dateTime().time();
	const QTime end_time = dateTime.time();
	update_duration_label(start_time, end_time);
	ui->btn_apply->setEnabled(true);
	ui->btn_apply->setText("Apply");
}
