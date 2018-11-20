#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

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

	bool add_issue(const Issue& issue);
	bool update_issue(const Issue& issue);
	bool remove_issue(const Issue& issue);
	IssueMap all_issues();
	Issue get_issue_by_id(Issue::Id id);

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
