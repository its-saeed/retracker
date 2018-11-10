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

	issue_timer.setInterval(1000);
	update_today_duration_timer.setInterval(1000 * 60);
	connect(&issue_timer, &QTimer::timeout, this, &MainWindow::on_issue_timer_timed_out);
	connect(&update_today_duration_timer, &QTimer::timeout, [this](){
		this->update_today_start();
		this->update_ui_today_time_and_efficiency();
	});
	update_today_duration_timer.start();

	connect(ui->tblw_doing_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_new_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_returned_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_qc_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btn_start_clicked()
{
	issue_timer.start();
	ui->btn_start->setEnabled(false);
	ui->btn_stop->setEnabled(true);
	ui->btn_dismiss->setEnabled(true);
	ui->btn_pause->setEnabled(true);
}

void MainWindow::on_issue_timer_timed_out()
{
	stop_watch.tick();
	ui->lbl_current_issue_timer->setText(QString::fromStdString(this->stop_watch.to_string()));
}

void MainWindow::on_btn_stop_clicked()
{
	issue_timer.stop();
	today_stats.add_useful_duration(stop_watch.get_minutes());
	issue_manager.add_duration(current_issue->get_id(), stop_watch.get_minutes());
	ui->lbl_total_useful_time->setText(today_stats.useful_duration_string());
	ui->prg_efficiency->setValue(today_stats.today_efficiency(QTime::currentTime()));
	stop_watch.reset();
	ui->lbl_current_issue_timer->setText("00:00:00");
	if (current_table)
		current_table->update_issue(*current_issue);

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
}

void MainWindow::on_btn_dismiss_clicked()
{
	int answer = QMessageBox::question(this, "Dismiss time slice", "You are about to dismiss current useful time. It means this time "
	"slice will not be calculated as useful time! Are you sure?");
	if (answer != QMessageBox::Yes)
		return;

	issue_timer.stop();
	ui->prg_efficiency->setValue(today_stats.today_efficiency(QTime::currentTime()));
	stop_watch.reset();

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
}

void MainWindow::on_btn_pause_clicked()
{
	issue_timer.stop();
	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(true);
	ui->btn_dismiss->setEnabled(true);
	ui->btn_pause->setEnabled(false);
}

void MainWindow::on_issue_selected(Issue::Id id)
{
	if (issue_timer.isActive())
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
	ui->lbl_current_issue_number->setText(QString("<a style=\"color: rgb(252, 175, 62);\" href=\"http://projects.mahsan.co/issues/%1\">#%1</a>").arg((int)id));
	ui->lbl_current_issue_number->setToolTip("Click to open issue.");
	ui->lbl_current_issue_number->setOpenExternalLinks(true);
	ui->lbl_current_issue_subject->setText(issue_manager.get_issue_by_id(id).get_subject());
	current_issue = &issue_manager.get_issue_by_id(id);

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
}

void MainWindow::on_btn_update_issues_clicked()
{
	UserPassDialog dlg;
	if (dlg.exec() == QDialog::Rejected)
		return;

	const QString username = dlg.get_user_name();
	const QString password = dlg.get_password();

	ui->btn_update_issues->setEnabled(false);
	QProcess process;
	process.start("/home/user/red.py", QStringList() << username << password);
	process.waitForFinished();
	ui->btn_update_issues->setEnabled(true);
	qDebug() << process.readAllStandardError();
	if (!issue_manager.load_from_file("/tmp/out.txt"))
	{
		QMessageBox::critical(this, "Load Issues", "Loading issues failed.");
		return;
	}

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
