#ifndef TIMESLICEEDITORDIALOG_H
#define TIMESLICEEDITORDIALOG_H

#include <QDialog>
#include "IssueManager.h"

namespace Ui {
class TimesliceEditorDialog;
}

class TimesliceEditorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TimesliceEditorDialog(QWidget *parent = nullptr);
	~TimesliceEditorDialog();
	void set_issue_manager(IssueManager* iss_m);
	void update_for_issue(Issue::Id id);

private slots:
	void on_tblw_timeslices_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

	void on_btn_apply_clicked();

	void on_dte_start_dateTimeChanged(const QDateTime &dateTime);
	void on_dte_end_dateTimeChanged(const QDateTime &dateTime);

	void on_btn_delete_timeslice_clicked();

	void on_btn_add_timeslice_clicked();

private:
	int get_current_timeslice_id() const;
	bool get_current_timeslice_applied_to_redmine() const;
	void update_duration_label(const QTime& start, const QTime& end);
	void remove_row(int row);
	void add_timeslice_to_table(const Timeslice& timeslice);
	Ui::TimesliceEditorDialog *ui;
	IssueManager* issue_manager;
	Issue::Id current_issue_id;
};

#endif // TIMESLICEEDITORDIALOG_H
