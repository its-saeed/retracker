#ifndef TODAYSUMMARYWIDGET_H
#define TODAYSUMMARYWIDGET_H

#include "Statistics.h"
#include "IssueManager.h"

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
	void set_issue_manager(const IssueManager* issue_manager)
	{
		this->issue_manager = issue_manager;
	}

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
	const IssueManager* issue_manager;
};

#endif // TODAYSUMMARYWIDGET_H
