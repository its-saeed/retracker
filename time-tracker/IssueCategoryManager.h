#ifndef ISSUECATEGORYMANAGER_H
#define ISSUECATEGORYMANAGER_H

#include <QWidget>
#include <map>

#include "IssueManager.h"
class IssueTableWidget;

namespace Ui {
class IssueCategoryManager;
}

class IssueCategoryManager : public QWidget
{
	Q_OBJECT

public:
	explicit IssueCategoryManager(QWidget *parent = nullptr);
	~IssueCategoryManager();

	void set_issue_manager(IssueManager* issue_man);
	void load_categories();
	bool filter_issues(const QString& filter);
	void focus();
public slots:
	void update_issue_tables();

private slots:
	void on_led_filter_returnPressed();
	void on_issue_manager_issue_updated(Issue::Id id);

	void on_btn_new_category_clicked();

	void on_tabw_issue_tables_tabCloseRequested(int index);

signals:
	void issue_selected(Issue::Id id);
	void issue_edit_requested(Issue::Id id);
private:
	bool find_issue_in(int table_index, IssueTableWidget* table, const QString& filter);
	void add_new_category_to_tab_widget(int catid, const QString& caption);
	void add_new_category(int catid, const QString& caption);
	Ui::IssueCategoryManager *ui;
	IssueManager* issue_manager;
	std::map<int, IssueTableWidget*> category_to_table_map;
};

#endif // ISSUECATEGORYMANAGER_H
