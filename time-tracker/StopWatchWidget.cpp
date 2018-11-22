#include "StopWatchWidget.h"
#include "ui_StopWatchWidget.h"

#include <QMessageBox>

StopWatchWidget::StopWatchWidget(QWidget *parent) :
QWidget(parent),
ui(new Ui::StopWatchWidget)
, pause(false)
{
	ui->setupUi(this);
	issue_timer.setInterval(1000);
	connect(&issue_timer, &QTimer::timeout, this, &StopWatchWidget::on_issue_timer_timeout);
}

StopWatchWidget::~StopWatchWidget()
{
	delete ui;
}

std::chrono::minutes StopWatchWidget::get_minutes() const
{
	return stop_watch.get_minutes();
}

std::chrono::seconds StopWatchWidget::get_seconds() const
{
	return stop_watch.get_seconds();
}

void StopWatchWidget::set_current_issue(const QString& issue_id, const QString& issue_subject)
{
	ui->lbl_current_issue_number->setText(QString("<a style=\"color: rgb(252, 175, 62);\" href=\"http://projects.mahsan.co/issues/%1\">#%1</a>").arg(issue_id));
	ui->lbl_current_issue_number->setToolTip("Click to open issue.");
	ui->lbl_current_issue_number->setOpenExternalLinks(true);
	ui->lbl_current_issue_subject->setText(issue_subject);

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
}

bool StopWatchWidget::is_active() const
{
	return issue_timer.isActive();
}

void StopWatchWidget::on_btn_start_clicked()
{
	// If timer stopped, restart ui and stop watch
	if (!pause)
	{
		ui->lbl_current_issue_timer->setText("00:00:00");
		stop_watch.reset();
	}

	pause = false;
	issue_timer.start();
	ui->btn_start->setEnabled(false);
	ui->btn_stop->setEnabled(true);
	ui->btn_dismiss->setEnabled(true);
	ui->btn_pause->setEnabled(true);
	emit started();
}

void StopWatchWidget::on_btn_stop_clicked()
{
	issue_timer.stop();
	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
	pause = false;
	emit stopped();
}

void StopWatchWidget::on_btn_dismiss_clicked()
{
	int answer = QMessageBox::question(this, "Dismiss time slice", "You are about to dismiss current useful time. It means this time "
	"slice will not be calculated as useful time! Are you sure?");
	if (answer != QMessageBox::Yes)
		return;

	issue_timer.stop();
	stop_watch.reset();

	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(false);
	ui->btn_dismiss->setEnabled(false);
	ui->btn_pause->setEnabled(false);
	pause = false;
	emit dismissed();
}

void StopWatchWidget::on_btn_pause_clicked()
{
	issue_timer.stop();
	ui->btn_start->setEnabled(true);
	ui->btn_stop->setEnabled(true);
	ui->btn_dismiss->setEnabled(true);
	ui->btn_pause->setEnabled(false);
	pause = true;
	emit paused();
}

void StopWatchWidget::on_issue_timer_timeout()
{
	stop_watch.tick();
	ui->lbl_current_issue_timer->setText(QString::fromStdString(this->stop_watch.to_string()));
	emit time_updated(QString::fromStdString(this->stop_watch.to_string()));
}
