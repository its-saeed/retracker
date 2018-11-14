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
	static constexpr const char* INSERT_TO_ISSUES_TABLE = "INSERT INTO issues values(%1, '%2', %3, %4)";
	QString query_string = QString(INSERT_TO_ISSUES_TABLE)
			.arg(issue.get_id())
			.arg(issue.get_state())
			.arg(issue.total_duration().count())
			.arg(issue.total_applied_duration().count());

	return execute_query(query_string);
}

bool DatabaseManager::update_issue(const Issue& issue)
{
	static constexpr const char* UPDATE_ISSUE_QUERY_STRING = "UPDATE issues SET subject = '%1', "
			"total_spent_time = %2, total_applied_to_redmine_time = %3 WHERE id = %4";
	QString query_string = QString(UPDATE_ISSUE_QUERY_STRING)
			.arg(issue.get_subject())
			.arg(issue.total_duration().count())
			.arg(issue.total_applied_duration().count());

	return execute_query(query_string);
}

bool DatabaseManager::remove_issue(const Issue& issue)
{
	static constexpr const char* DELETE_FROM_ORDERS_TABLE = "DELETE FROM issues WHERE id=%!;";

	QString query_string = QString(DELETE_FROM_ORDERS_TABLE).arg(issue.get_id());
	return execute_query(query_string);

}

IssueMap DatabaseManager::all_issues()
{
	static constexpr const char* SELECT_ORDERS = "SELECT order_id,order_time,respose_time, response_total_bytes,"
		"order_additional_info,order_type FROM orders WHERE client_uid = '%1'";
	QSqlQuery query;
	//query.exec(QString(SELECT_ORDERS).arg(client_uniq_id));

	//while(query.next())
	return IssueMap();
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
	last_error = db.lastError().text() + QSqlDatabase::drivers().join(" ");
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
