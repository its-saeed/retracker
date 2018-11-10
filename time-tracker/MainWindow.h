#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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

public slots:
	void on_btn_update_issues_clicked();
private slots:
	void on_issue_selected(Issue::Id id);
	void on_btn_change_day_start_clicked();

	void on_stop_watch_stopped();
	void on_stop_watch_dismissed();

private:
	void update_ui_today_time_and_efficiency();
	void update_issue_tables();
	void update_today_start();
	void change_today_start_to(const QDateTime& today);
	Ui::MainWindow *ui;
	QTimer update_today_duration_timer;
	Statistics today_stats;
	IssueManager issue_manager;
	Issue* current_issue;
	IssueTableWidget* current_table;
};

#endif // MAINWINDOW_H
