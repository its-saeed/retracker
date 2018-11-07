#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
, current_table(nullptr)
, current_issue(nullptr)
{
	ui->setupUi(this);
	today_stats.set_day_start(QTime::currentTime());
	ui->lbl_today_start->setText(today_stats.day_start_string());
	issue_timer.setInterval(1000);
	update_today_duration_timer.setInterval(1000 * 60);
	connect(&issue_timer, &QTimer::timeout, this, &MainWindow::on_issue_timer_timed_out);
	connect(&update_today_duration_timer, &QTimer::timeout, [this](){
		this->ui->lbl_today_duration->setText(this->today_stats.today_duration_string(QTime::currentTime()));
		this->ui->prg_efficiency->setValue(today_stats.today_efficiency(QTime::currentTime()));
	});
	update_today_duration_timer.start();

	connect(ui->tblw_doing_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_new_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_returned_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);
	connect(ui->tblw_qc_issues, &IssueTableWidget::issue_selected, this, &MainWindow::on_issue_selected);

	Issue issue(111, "slam", Issue::NEW);
	ui->tblw_new_issues->add_issue(issue);
	issue_manager.add_issue(issue);
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
//		return;
	}

	current_table = reinterpret_cast<IssueTableWidget*>(sender());
	ui->lbl_current_issue_number->setText(QString("<a style=\"color: rgb(252, 175, 62);\" href=\"http://projects.mahsan.co/issues/%1\">#%1</a>").arg((int)id));
	ui->lbl_current_issue_number->setToolTip("Click to open issue.");
	ui->lbl_current_issue_number->setOpenExternalLinks(true);
	current_issue = &issue_manager.get_issue_by_id(id);

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
}
