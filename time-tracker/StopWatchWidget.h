#ifndef STOPWATCHWIDGET_H
#define STOPWATCHWIDGET_H

#include <QTimer>
#include <QWidget>
#include <QDateTime>
#include <Timer.h>
#include "Issue.h"

namespace Ui {
class StopWatchWidget;
}

class StopWatchWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StopWatchWidget(QWidget *parent = 0);
	~StopWatchWidget();

	std::chrono::minutes get_minutes() const;
	std::chrono::seconds get_seconds() const;
	Timeslice get_timeslice() const;
	void set_current_issue(const QString& issue_id, const QString& issue_subject);
	bool is_active() const;

private:
	Ui::StopWatchWidget *ui;
	QTimer issue_timer;
	StopWatch stop_watch;
	bool pause;
	QDateTime start, end;

signals:
	void stopped();
	void paused();
	void started();
	void dismissed();
	void time_updated(const QString timestr);

public slots:
	void on_btn_stop_clicked();
	void on_btn_start_clicked();
private slots:
	void on_btn_pause_clicked();
	void on_btn_dismiss_clicked();
	void on_issue_timer_timeout();
};

#endif // STOPWATCHWIDGET_H
