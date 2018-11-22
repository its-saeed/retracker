#ifndef TODAYSUMMARYWIDGET_H
#define TODAYSUMMARYWIDGET_H

#include "Statistics.h"
#include <QWidget>
#include <QTimer>

namespace Ui {
class TodaySummaryWidget;
}

class TodaySummaryWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TodaySummaryWidget(QWidget *parent = nullptr);
	~TodaySummaryWidget();
	void change_today_start_to(const QDateTime& today);
	void add_useful_time_slice(const std::chrono::minutes& mins);

public slots:
	void update();
private slots:
	void on_btn_change_day_start_clicked();
private:
	void set_today_start_time(const QString& day_start);
	void set_today_work_durations(const QString& total, const QString useful, int useful_percent);
	void update_today_start();

	Ui::TodaySummaryWidget *ui;
	Statistics today_stats;
	QTimer update_today_duration_timer;
};

#endif // TODAYSUMMARYWIDGET_H
