#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QDateTime>
#include <QString>
#include <QtSql/QSqlDatabase>

#include "Issue.h"

class DatabaseManager
{
public:
	DatabaseManager();
	static DatabaseManager& instance();
	bool open_database(const QString& path);
	bool drop_database();
	QString get_last_error() const;
	bool is_connected() const;

	bool add_issue(Issue& issue);
	int add_time_slice(Issue::Id id, const Timeslice& timeslice);
	bool update_timeslice(const Timeslice& ts);
	bool remove_timeslice(int id);
	bool set_issue_timeslices_applied_to_redmine(Issue::Id id);
	bool update_issue(const Issue& issue);
	bool update_issues(const IssueMap& issue_map);
	bool add_issues(IssueMap& issue_map);
	bool remove_issue(Issue::Id id);
	IssueMap all_issues();
	Issue get_issue_by_id(Issue::Id id);
	TimesliceVector get_issue_timeslices(Issue::Id id);

	bool add_category(int id, const QString& cat_text);
	std::map<int, QString> all_categories();
	bool remove_category(int id);

private:
	static constexpr const char* DB_NAME = "retracker";
	bool add_database(const QString& path);
	bool create_database();
	bool create_tables();
	bool execute_query(const QString& query_string);
	bool is_open;
	QString last_error;
	QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
