#include "ui_TodaySummaryWidget.h"

#include "TodaySummaryWidget.h"
#include "TimePickerDialog.h"
#include "Settings.h"
#include "commons.h"

TodaySummaryWidget::TodaySummaryWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TodaySummaryWidget)
{
	ui->setupUi(this);
	update_today_start();

	update_today_duration_timer.setInterval(1000 * 60);
	connect(&update_today_duration_timer, &QTimer::timeout, [this](){
		this->update_today_start();
		this->update();
	});
	update_today_duration_timer.start();

}

TodaySummaryWidget::~TodaySummaryWidget()
{
	delete ui;
}

void TodaySummaryWidget::update()
{
	set_today_start_time(today_stats.day_start_string());
	set_today_work_durations(today_stats.today_duration_string(QTime::currentTime()),
							 QString::fromStdString(commons::to_string(issue_manager->get_total_useful_durations(QDate::currentDate()))),
							 static_cast<int>(today_stats.today_efficiency(QTime::currentTime())));
}

void TodaySummaryWidget::set_today_start_time(const QString& day_start)
{
	ui->lbl_today_start->setText(day_start);
}

void TodaySummaryWidget::set_today_work_durations(const QString& total, const QString useful, int useful_percent)
{
	ui->lbl_today_duration->setText(total);
	ui->lbl_total_useful_time->setText(useful);
	ui->prg_efficiency->setValue(useful_percent);
}

void TodaySummaryWidget::on_btn_change_day_start_clicked()
{
	QDateTime today = QDateTime::currentDateTime();
	TimePickerDialog dlg;
	if (dlg.exec() == QDialog::Rejected)
		return;

	QTime user_entered_time = dlg.get_time();
	today.setTime(user_entered_time);
	change_today_start_to(today);
	update();
}

void TodaySummaryWidget::change_today_start_to(const QDateTime& today)
{
	Settings::instance().save_today(today);
	today_stats.set_day_start(today.time());
}

void TodaySummaryWidget::update_today_start()
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
