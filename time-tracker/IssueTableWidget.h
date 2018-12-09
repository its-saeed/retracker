#ifndef ISSUETABLEWIDGET_H
#define ISSUETABLEWIDGET_H

#include <QTableWidget>
#include "Issue.h"
#include <QMap>

class IssueTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	static constexpr int ISSUE_ID_COLUMN = 0;
	static constexpr int ISSUE_SUBJECT_COLUMN = 1;
	static constexpr int ISSUE_TOTAL_TIME_COLUMN = 2;
	static constexpr int ISSUE_TOTAL_APPLIED_TIME_COLUMN = 3;
	static constexpr int ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN = 4;
	IssueTableWidget(QWidget* parent = nullptr);
	void add_issue(const Issue& issue);
	void update_issue(const Issue& issue);

private slots:
	void on_context_menu_requested(const QPoint &pos);
	void item_clicked(int row, int col);
	void on_item_double_clicked(int row, int col);
signals:
	void issue_selected(Issue::Id id);
	void issue_double_clicked(Issue::Id id);
private:
	Issue::Id get_currently_selected_issue_id() const;
	QMap<Issue::Id, int> issue_id_to_table_row_map;
};

#endif // ISSUETABLEWIDGET_H
