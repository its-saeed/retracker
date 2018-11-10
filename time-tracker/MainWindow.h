#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "Timer.h"
#include "Statistics.h"
#include "IssueManager.h"

class IssueTableWidget;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_btn_start_clicked();
	void on_issue_timer_timed_out();
	void on_btn_stop_clicked();
	void on_btn_dismiss_clicked();
	void on_btn_pause_clicked();
	void on_issue_selected(Issue::Id id);

	void on_btn_update_issues_clicked();

	void on_btn_change_day_start_clicked();

private:
	void update_ui_today_time_and_efficiency();
	void update_issue_tables();
	void update_today_start();
	void change_today_start_to(const QDateTime& today);
	Ui::MainWindow *ui;
	QTimer issue_timer;
	QTimer update_today_duration_timer;
	StopWatch stop_watch;
	Statistics today_stats;
	IssueManager issue_manager;
	Issue* current_issue;
	IssueTableWidget* current_table;
};

#endif // MAINWINDOW_H
