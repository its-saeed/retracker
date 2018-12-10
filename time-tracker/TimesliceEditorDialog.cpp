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
	ui->de_add_end->setDate(QDate::currentDate());
	ui->de_add_start->setDate(QDate::currentDate());
	ui->te_add_end->setTime(QTime::currentTime());
	ui->te_add_start->setTime(QTime::currentTime());

	ui->de_update_end->setDate(QDate::currentDate());
	ui->de_update_start->setDate(QDate::currentDate());
	ui->te_update_end->setTime(QTime::currentTime());
	ui->te_update_start->setTime(QTime::currentTime());
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

	for (auto& kv : issue.get_timeslices())
		add_timeslice_to_table(kv.second);
}
void TimesliceEditorDialog::add_timeslice_to_table(const Timeslice& timeslice)
{
	int row = ui->tblw_timeslices->rowCount();
	int col = 0;
	ui->tblw_timeslices->setRowCount(row + 1);
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
}


void TimesliceEditorDialog::on_tblw_timeslices_currentCellChanged(int currentRow,
																  int, int, int)
{
	QDateTime start_date_time(QDate::fromString(ui->tblw_timeslices->item(currentRow, 1)->text()),
							  QTime::fromString(ui->tblw_timeslices->item(currentRow, 2)->text()));

	QDateTime end_date_time(QDate::fromString(ui->tblw_timeslices->item(currentRow, 3)->text()),
							  QTime::fromString(ui->tblw_timeslices->item(currentRow, 4)->text()));

	ui->te_update_start->setTime(start_date_time.time());
	ui->de_update_start->setDate(start_date_time.date());
	ui->te_update_end->setTime(end_date_time.time());
	ui->de_update_end->setDate(end_date_time.date());

	ui->te_add_start->setTime(start_date_time.time());
	ui->te_add_end->setTime(end_date_time.time());
	ui->de_add_start->setDate(start_date_time.date());
	ui->de_add_end->setDate(end_date_time.date());

	update_duration_label(start_date_time.time(), end_date_time.time());
}

void TimesliceEditorDialog::on_btn_apply_clicked()
{
	if (get_current_timeslice_id() == 0)
		return;

	const QTime& start_time = ui->te_update_start->time();
	const QDate& start_date = ui->de_update_start->date();
	const QTime& end_time = ui->te_update_end->time();
	const QDate& end_date = ui->de_update_end->date();

	if (start_date.daysTo(end_date) != 0)
	{
		QMessageBox::warning(this, "Apply", "Start and end date should be in the same date!");
		return;
	}

	if (start_time > end_time)
	{
		QMessageBox::warning(this, "Apply", "Start is ahead of the end !");
		return;
	}

	Timeslice ts{get_current_timeslice_id(), QDateTime(start_date, start_time), QDateTime(end_date, end_time),
				get_current_timeslice_applied_to_redmine()};
	if (!issue_manager->update_timeslice(current_issue_id, ts) || !DatabaseManager::instance().update_timeslice(ts))
	{
		QMessageBox::warning(this, "Error", "Couldn't update the timeslice");
		return;
	}

	ui->btn_apply->setText("Applied...");
	ui->btn_apply->setEnabled(false);

	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 1)->setText(start_date.toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 2)->setText(start_time.toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 3)->setText(end_date.toString());
	ui->tblw_timeslices->item(ui->tblw_timeslices->currentRow(), 4)->setText(end_time.toString());
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
	const QTime end_time = ui->te_update_end->dateTime().time();
	update_duration_label(start_time, end_time);
	ui->btn_apply->setEnabled(true);
	ui->btn_apply->setText("Apply");
}

void TimesliceEditorDialog::on_dte_end_dateTimeChanged(const QDateTime& dateTime)
{
	const QTime start_time = ui->te_update_start->dateTime().time();
	const QTime end_time = dateTime.time();
	update_duration_label(start_time, end_time);
	ui->btn_apply->setEnabled(true);
	ui->btn_apply->setText("Apply");
}

void TimesliceEditorDialog::on_btn_delete_timeslice_clicked()
{
	int answer = QMessageBox::question(this, "Delete timeslice", "You are about to delete currently selecte timeslice! Are you sure?");
	if (answer != QMessageBox::Yes)
		return;

	if (!issue_manager->delete_timeslice(current_issue_id, get_current_timeslice_id())
			|| !DatabaseManager::instance().remove_timeslice(get_current_timeslice_id()))
	{
		QMessageBox::warning(this, "Delete Timeslice", "Coulnd't delete the timeslice!");
		return;
	}

	remove_row(ui->tblw_timeslices->currentRow());
}

void TimesliceEditorDialog::remove_row(int row)
{
	for (int i = 0; i < ui->tblw_timeslices->columnCount(); ++i)
		delete ui->tblw_timeslices->item(row, i);

	ui->tblw_timeslices->removeRow(ui->tblw_timeslices->currentRow());
}

void TimesliceEditorDialog::on_btn_add_timeslice_clicked()
{
	if (get_current_timeslice_id() == 0)
		return;

	const QTime& start_time = ui->te_add_start->time();
	const QDate& start_date = ui->de_add_start->date();
	const QTime& end_time = ui->te_add_end->time();
	const QDate& end_date = ui->de_add_end->date();

	if (start_date.daysTo(end_date) != 0)
	{
		QMessageBox::warning(this, "Add", "Start and end date should be in the same date!");
		return;
	}

	if (start_time > end_time)
	{
		QMessageBox::warning(this, "Add", "Start is ahead of the end !");
		return;
	}

	Timeslice ts{get_current_timeslice_id(), QDateTime(start_date, start_time), QDateTime(end_date, end_time), false};
	int ts_id = issue_manager->add_timeslice(current_issue_id, ts);
	ts.id = ts_id;

	if (ts_id == -1)
	{
		QMessageBox::warning(this, "Error", "Couldn't add the timeslice");
		return;
	}

	add_timeslice_to_table(ts);
}
