#ifndef STOPWATCHWIDGET_H
#define STOPWATCHWIDGET_H

#include <QTimer>
#include <QWidget>
#include <Timer.h>

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
	void set_current_issue(const QString& issue_id, const QString& issue_subject);
	bool is_active() const;

private:
	Ui::StopWatchWidget *ui;
	QTimer issue_timer;
	StopWatch stop_watch;
	bool pause;

signals:
	void stopped();
	void paused();
	void started();
	void dismissed();
	void time_updated(const QString timestr);
private slots:
	void on_btn_stop_clicked();
	void on_btn_start_clicked();
	void on_btn_pause_clicked();
	void on_btn_dismiss_clicked();
	void on_issue_timer_timed_out();
};

#endif // STOPWATCHWIDGET_H
