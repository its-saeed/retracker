#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QInputDialog>
#include <QKeyEvent>

#include "UserPassDialog.h"
#include "TimePickerDialog.h"
#include "ApplyTimeToPaygirDialog.h"
#include "commons.h"
#include "Settings.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
, current_issue(nullptr)
, current_table(nullptr)
{
	ui->setupUi(this);
	update_today_start();
	update_ui_today_time_and_efficiency();

	update_today_duration_timer.setInterval(1000 * 60);
	connect(&update_today_duration_timer, &QTimer::timeout, [this](){
		this->update_today_start();
		this->update_ui_today_time_and_efficiency();
	});
	update_today_duration_timer.start();

	connect(ui->tblw_doing_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_new_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_returned_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_qc_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->wdg_stop_watch, &StopWatchWidget::stopped, this, &MainWindow::on_stop_watch_stopped);
	connect(ui->wdg_stop_watch, &StopWatchWidget::dismissed, this, &MainWindow::on_stop_watch_dismissed);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_issue_selected(Issue::Id id)
{
	if (ui->wdg_stop_watch->is_active())
	{
		QMessageBox::warning(this, "Tracker", "Please stop current timer to start working on new issue!");
		return;
	}

	if (!issue_manager.issue_exists(id))
	{
		QMessageBox::critical(this, "Errro", "Surprisingly issue doesn't exist. It definitly a bug!");
		return;
	}

	current_table = reinterpret_cast<IssueTableWidget*>(sender());
	current_issue = &issue_manager.get_issue_by_id(id);
	ui->wdg_stop_watch->set_current_issue(QString::number(current_issue->get_id()), current_issue->get_subject());
}

void MainWindow::on_btn_update_issues_clicked()
{
	UserPassDialog dlg(this);
	if (dlg.exec() == QDialog::Rejected)
		return;

	const QString username = dlg.get_user_name();
	const QString password = dlg.get_password();

	ui->btn_update_issues->setEnabled(false);
	setCursor(Qt::BusyCursor);
	QProcess process;
	process.start("scripts/red.py", QStringList() << username << password);
	process.waitForFinished();
	setCursor(Qt::ArrowCursor);
	qDebug() << process.readAllStandardError();
	if (!issue_manager.load_from_file(".issues.txt"))
	{
		QMessageBox::critical(this, "Load Issues", "Loading issues failed.");
		ui->btn_update_issues->setEnabled(true);
		return;
	}

	ui->btn_update_issues->setEnabled(true);
	update_issue_tables();
}

void MainWindow::update_ui_today_time_and_efficiency()
{
	ui->lbl_today_start->setText(this->today_stats.day_start_string());
	ui->lbl_today_duration->setText(this->today_stats.today_duration_string(QTime::currentTime()));
	ui->prg_efficiency->setValue(today_stats.today_efficiency(QTime::currentTime()));
	ui->lbl_total_useful_time->setText(today_stats.useful_duration_string());
}

void MainWindow::update_issue_tables()
{
	for (auto kv : issue_manager.get_issues())
	{
		const Issue& issue = kv.second;
		if (issue.get_state() == Issue::State::NEW)
			ui->tblw_new_issues->add_issue(issue);
		else if (issue.get_state() == Issue::State::DOING)
			ui->tblw_doing_issues->add_issue(issue);
		else if (issue.get_state() == Issue::State::RETURNED)
			ui->tblw_returned_issues->add_issue(issue);
		else if (issue.get_state() == Issue::State::QUALITY_CHECK)
			ui->tblw_qc_issues->add_issue(issue);
	}
}

void MainWindow::update_today_start()
{
	QDateTime current = QDateTime::currentDateTime();
	QDateTime prev = Settings::instance().get_today();
	QDateTime today = commons::get_today(current, prev);
	if (!today.isValid())
	{
		TimePickerDialog dlg;
		if (dlg.exec() == QDialog::Rejected)
			return;

		QTime user_entered_time = dlg.get_time();
		today.setTime(user_entered_time);
	}
	change_today_start_to(today);
}

void MainWindow::change_today_start_to(const QDateTime& today)
{
	Settings::instance().save_today(today);
	today_stats.set_day_start(today.time());
}

bool MainWindow::load_issue_from_peygir(int issue_id)
{
	const QString username = Settings::instance().get_username();
	const QString password = Settings::instance().get_password();

	QProcess process;
	process.start("scripts/find_issue.py", QStringList() << username << password << QString::number(issue_id));
	process.waitForFinished();
	if (!issue_manager.load_from_file(".issues.txt"))
		return false;

	return true;
}

bool MainWindow::filter_issues(const QString& filter)
{
	if (filter.isEmpty())
		return false;

	if (find_issue_in(0, ui->tblw_new_issues, filter))
		return true;

	if (find_issue_in(1, ui->tblw_doing_issues, filter))
		return true;

	if (find_issue_in(2, ui->tblw_returned_issues, filter))
		return true;

	if (find_issue_in(3, ui->tblw_qc_issues, filter))
		return true;

	return false;		// No Issue found with this criteria
}

void MainWindow::on_led_filter_returnPressed()
{
	if (filter_issues(this->ui->led_filter->text()))
		return;

	bool is_num;
	int issue_id = ui->led_filter->text().toInt(&is_num);
	if (!issue_id)
		return;

	if (!ui->chb_add_if_not_exist->isChecked())
		return;

	if (this->load_issue_from_peygir(issue_id))
	{
		update_issue_tables();		// TODO: change with more efficient method
		filter_issues(QString::number(issue_id));
	}
	ui->led_filter->selectAll();
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_F && QApplication::keyboardModifiers() && Qt::ControlModifier)
	{
		ui->led_filter->setFocus();
		ui->led_filter->selectAll();
	}
	else
		QMainWindow::keyPressEvent(event);
}

bool MainWindow::find_issue_in(int table_index, QTableWidget* table, const QString& filter)
{
	auto items = table->findItems(filter, Qt::MatchContains);
	if (items.size() > 0)
	{
		ui->tabw_issue_tables->setCurrentIndex(table_index);
		table->selectRow(items.at(0)->row());
		return true;
	}

	return false;
}

void MainWindow::on_btn_change_day_start_clicked()
{
	QDateTime today = QDateTime::currentDateTime();
	TimePickerDialog dlg;
	if (dlg.exec() == QDialog::Rejected)
		return;

	QTime user_entered_time = dlg.get_time();
	today.setTime(user_entered_time);
	change_today_start_to(today);
	update_ui_today_time_and_efficiency();
}

void MainWindow::on_stop_watch_stopped()
{
	std::chrono::minutes ellapsed_minutes = ui->wdg_stop_watch->get_minutes();
	today_stats.add_useful_duration(ellapsed_minutes);
	issue_manager.add_duration(current_issue->get_id(), ellapsed_minutes);
	update_ui_today_time_and_efficiency();
	if (current_table)
		current_table->update_issue(*current_issue);

}

void MainWindow::on_stop_watch_dismissed()
{
	update_ui_today_time_and_efficiency();
}

void MainWindow::on_btn_apply_times_clicked()
{
	ApplyTimeToPaygirDialog dlg(issue_manager);
	dlg.exec();
}

void MainWindow::on_btn_add_issue_clicked()
{
	int issue_id = QInputDialog::getInt(this, "Issue ID", tr("لطفا شماره مسئله را وارد کنید."), 0,  0);


	ui->btn_add_issue->setEnabled(false);
	setCursor(Qt::BusyCursor);
	bool loaded = load_issue_from_peygir(issue_id);
	setCursor(Qt::ArrowCursor);
	if (!loaded)
	{
		QMessageBox::critical(this, "Load Issue", QString("Loading issue #%1 failed.").arg(issue_id));
		ui->btn_add_issue->setEnabled(true);
		return;
	}

	ui->btn_add_issue->setEnabled(true);
	update_issue_tables();		// TODO: change it with more efficient method
	filter_issues(QString::number(issue_id));
}
