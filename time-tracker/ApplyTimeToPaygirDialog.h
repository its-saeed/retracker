#ifndef APPLYTIMETOPAYGIRDIALOG_H
#define APPLYTIMETOPAYGIRDIALOG_H

#include <QDialog>
#include <IssueManager.h>
#include <QTableWidgetItem>

namespace Ui {
class ApplyTimeToPaygirDialog;
}

class ApplyTimeToPaygirDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ApplyTimeToPaygirDialog(IssueManager& manager, QWidget *parent = 0);
	~ApplyTimeToPaygirDialog();

private slots:
	void on_btn_apply_times_clicked();

	void on_chb_select_all_toggled(bool checked);

	void on_btn_today_clicked();

private:
	static constexpr int ISSUE_ID_COLUMN = 0;
	static constexpr int ISSUE_SUBJECT_COLUMN = 1;
	static constexpr int ISSUE_TOTAL_TIME_COLUMN = 2;
	static constexpr int ISSUE_PERFECT_TIME_COLUMN = 3;
	static constexpr int ISSUE_COMMENTS_COLUMN = 4;

	void update_issues();
	void add_issue_to_table(const Issue& issue);
	void add_time_entry_to_peygir(const QString& issue_id, const QString& total_time, const QString& total_perfect_time,
	const QString& comment);

	Ui::ApplyTimeToPaygirDialog *ui;
	IssueManager& issue_manager;
};

#endif // APPLYTIMETOPAYGIRDIALOG_H
