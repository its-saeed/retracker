#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btn_start_clicked()
{
	issue_timer.start();
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
	ui->lbl_total_useful_time->setText(today_stats.useful_duration_string());
	ui->prg_efficiency->setValue(today_stats.today_efficiency(QTime::currentTime()));
	stop_watch.reset();
	ui->lbl_current_issue_timer->setText("00:00:00");
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
}

void MainWindow::on_btn_pause_clicked()
{
	issue_timer.stop();
}
