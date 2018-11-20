#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>

DatabaseManager::DatabaseManager()
	: is_open(false)
{
}

DatabaseManager&DatabaseManager::instance()
{
	static DatabaseManager manager;
	return manager;
}

bool DatabaseManager::open_database(const QString& path)
{
	if (is_open)
		return true;

	if (!add_database(path))
		return false;

	if (!create_tables())
		return false;

	is_open = true;
	return is_open;
}

bool DatabaseManager::create_database()
{
	constexpr const char* CREATE_DATABASE_QUERY = "CREATE DATABASE IF NOT EXISTS %1;";

	QString query_string = QString(CREATE_DATABASE_QUERY).arg(DB_NAME);
	return execute_query(query_string);
}

bool DatabaseManager::create_tables()
{
	const QString issues_table_query("CREATE TABLE issues ("
														  "id	INTEGER NOT NULL UNIQUE,"
														  "subject	TEXT NOT NULL,"
														  "state INTEGER,"
														  "total_spent_time	INTEGER,"
														  "total_applied_to_redmine_time	INTEGER,"
														  "tags	TEXT,"
														  "PRIMARY KEY(id)"
													  ");");

	// First create clients table
	if (!execute_query(issues_table_query))
		return false;

	return true;

}

bool DatabaseManager::drop_database()
{
	QString query_string("drop database `%1`;");
	query_string = query_string.arg(DB_NAME);
	if (!execute_query(query_string))
		return false;
	return create_database();
}

bool DatabaseManager::add_issue(const Issue& issue)
{
	static constexpr const char* INSERT_TO_ISSUES_TABLE = "INSERT INTO issues values(%1, '%2', %3, %4, %5, '')";
	QString query_string = QString(INSERT_TO_ISSUES_TABLE)
			.arg(issue.get_id())
			.arg(issue.get_subject())
			.arg(issue.get_state())
			.arg(issue.total_duration().count())
			.arg(issue.total_applied_duration().count());

	return execute_query(query_string);
}

bool DatabaseManager::update_issue(const Issue& issue)
{
	static constexpr const char* UPDATE_ISSUE_QUERY_STRING = "UPDATE issues SET subject = '%1', "
			"total_spent_time = %2, total_applied_to_redmine_time = %3, state = %4 WHERE id = %5";
	QString query_string = QString(UPDATE_ISSUE_QUERY_STRING)
			.arg(issue.get_subject())
			.arg(issue.total_duration().count())
			.arg(issue.total_applied_duration().count())
			.arg(issue.get_state())
			.arg(issue.get_id());

	return execute_query(query_string);
}

bool DatabaseManager::remove_issue(Issue::Id id)
{
	static constexpr const char* DELETE_FROM_ORDERS_TABLE = "DELETE FROM issues WHERE id=%1;";

	QString query_string = QString(DELETE_FROM_ORDERS_TABLE).arg(id);
	return execute_query(query_string);

}

IssueMap DatabaseManager::all_issues()
{
	IssueMap map;
	constexpr const char* SELECT_ISSUE = "SELECT id, subject, state, total_spent_time, total_applied_to_redmine_time FROM issues";
	QSqlQuery query;
	if (!query.exec(QString(SELECT_ISSUE)))
		return map;

	while (query.next())
	{
		Issue iss;
		iss.set_id(query.value(0).toInt());
		iss.set_subject(query.value(1).toString());
		iss.set_state(static_cast<Issue::State>(query.value(2).toInt()));
		iss.set_total_spent_time(std::chrono::minutes{query.value(3).toInt()});
		iss.set_total_applied_to_redmine_time(std::chrono::minutes{query.value(4).toInt()});
		map[iss.get_id()] = iss;

	}

	return map;
}

Issue DatabaseManager::get_issue_by_id(Issue::Id id)
{
	constexpr const char* SELECT_ISSUE = "SELECT subject, state, total_spent_time, total_applied_to_redmine_time"
										 " FROM issues WHERE id = %1";
	QSqlQuery query;
	if (!query.exec(QString(SELECT_ISSUE).arg(id)))
		return Issue();

	if (!query.next())
		return Issue();

	Issue iss;
	iss.set_id(id);
	iss.set_subject(query.value(0).toString());
	iss.set_state(static_cast<Issue::State>(query.value(1).toInt()));
	iss.set_total_spent_time(std::chrono::minutes{query.value(2).toInt()});
	iss.set_total_applied_to_redmine_time(std::chrono::minutes{query.value(3).toInt()});

	return iss;
}

QString DatabaseManager::get_last_error() const
{
	return last_error;
}

bool DatabaseManager::is_connected() const
{
	return is_open;
}

bool DatabaseManager::add_database(const QString& path)
{
	// First try to open database
	db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(path);

	bool open = db.open();
//	last_error = db.lastError().text();
	return open;
}

bool DatabaseManager::execute_query(const QString& query_string)
{
	static int i = 0;
	QSqlQuery query;

	if (query.exec(query_string))
	{
		i = 0;
		return true;
	}

	++i;
	if (i < 3)
		return execute_query(query_string);

	i = 0;
	last_error = query.lastError().text() + " " + query_string;
	return false;
}
