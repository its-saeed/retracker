#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include <QFile>

#include "UserPassDialog.h"
#include "TimePickerDialog.h"
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
	process.start("/home/user/red.py", QStringList() << username << password);
	process.waitForFinished();
	setCursor(Qt::ArrowCursor);
	qDebug() << process.readAllStandardError();
	if (!issue_manager.load_from_file("/tmp/out.txt"))
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
