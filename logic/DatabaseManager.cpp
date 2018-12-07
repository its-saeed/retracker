#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QStringList>
#include <QVariant>
#include <QString>

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
	const QString issues_table_query("CREATE TABLE IF NOT EXISTS issues ("
														  "id	INTEGER NOT NULL UNIQUE,"
														  "subject	TEXT NOT NULL,"
														  "state INTEGER,"
														  "tags	TEXT,"
														  "PRIMARY KEY(id)"
													  ");");

	const QString timeslice_table_query("CREATE TABLE IF NOT EXISTS timeslice ("
										"id	INTEGER NOT NULL UNIQUE PRIMARY KEY,"
										"issue_id	INTEGER NOT NULL,"
										"start	TEXT NOT NULL,"
										"end	INTEGER NOT NULL,"
										"applied_to_redmine	INTEGER NOT NULL DEFAULT 0,"
										"FOREIGN KEY(issue_id) REFERENCES issues(id) on delete cascade on update cascade);");

	const QString category_table_query("CREATE TABLE IF NOT EXISTS categories ("
									   "id	INTEGER NOT NULL UNIQUE,"
									   "cat_text	TEXT NOT NULL,"
									   "PRIMARY KEY(id)"
									   ");");
	// First create clients table
	if (!execute_query(issues_table_query))
		return false;

	if (!execute_query(timeslice_table_query))
		return false;

	if (!execute_query(category_table_query))
		return false;

	add_category(Issue::State::NEW, "جدید");
	add_category(Issue::State::DOING, "درحال انجام");
	add_category(Issue::State::RETURNED, "بازخورد");
	add_category(Issue::State::QUALITY_CHECK, "بررسی کیفیت");

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

bool DatabaseManager::add_issue(Issue& issue)
{
	static constexpr const char* INSERT_TO_ISSUES_TABLE = "INSERT INTO issues values(%1, '%2', %3, '')";
	QString query_string = QString(INSERT_TO_ISSUES_TABLE)
			.arg(issue.get_id())
			.arg(issue.get_subject())
			.arg(issue.get_state());

	bool insertion = execute_query(query_string);
	if (!insertion)
	{
		last_error = QString("Issue with ID: %1 couldn't be added.").arg(issue.get_id());
		return false;
	}

	for (auto& kv : issue.get_timeslices())
	{
		Timeslice& timeslice = kv.second;
		int ts_id = add_time_slice(issue.get_id(), timeslice);
		if (ts_id == -1)
		{
			last_error = QString("Issue time slice couldn't be added.");
			return false;
		}

		timeslice.id = ts_id;
	}

	return true;
}

int DatabaseManager::add_time_slice(Issue::Id id, const Timeslice& timeslice)
{
	QString query_string = QString("INSERT INTO timeslice VALUES (NULL, %1, '%2', '%3', %4)")
			.arg(id)
			.arg(timeslice.start.toString())
			.arg(timeslice.end.toString())
			.arg(static_cast<int>(timeslice.applied_to_redmine));

	QSqlQuery query;
	if (!query.exec(query_string))
		return -1;

	return query.lastInsertId().toInt();
}

bool DatabaseManager::update_timeslice(const Timeslice& ts)
{
	static constexpr const char* UPDATE_ISSUE_QUERY_STRING = "UPDATE timeslice SET start = '%1', end = '%2', applied_to_redmine = %3 "
			" WHERE id = %4";
	QString query_string = QString(UPDATE_ISSUE_QUERY_STRING)
			.arg(ts.start.toString())
			.arg(ts.end.toString())
			.arg(ts.applied_to_redmine)
			.arg(ts.id);

	return execute_query(query_string);
}

bool DatabaseManager::remove_timeslice(int id)
{
	static constexpr const char* DELETE_FROM_ORDERS_TABLE = "DELETE FROM timeslice WHERE id=%1;";

	QString query_string = QString(DELETE_FROM_ORDERS_TABLE).arg(id);
	return execute_query(query_string);
}

bool DatabaseManager::set_issue_timeslices_applied_to_redmine(Issue::Id id)
{
	static constexpr const char* UPDATE_ISSUE_QUERY_STRING = "UPDATE timeslice SET applied_to_redmine = 1 "
			" WHERE issue_id = %4";
	QString query_string = QString(UPDATE_ISSUE_QUERY_STRING).arg(id);

	return execute_query(query_string);
}

bool DatabaseManager::update_issue(const Issue& issue)
{
	static constexpr const char* UPDATE_ISSUE_QUERY_STRING = "UPDATE issues SET subject = '%1', "
			" state = %2 WHERE id = %3";
	QString query_string = QString(UPDATE_ISSUE_QUERY_STRING)
			.arg(issue.get_subject())
			.arg(issue.get_state())
			.arg(issue.get_id());

	return execute_query(query_string);
}

bool DatabaseManager::update_issues(const IssueMap& issue_map)
{
	bool update = true;
	for (const auto& i : issue_map)
	{
		const Issue& issue = i.second;
		update = update && update_issue(issue);
	}

	return update;
}

bool DatabaseManager::add_issues(IssueMap& issue_map)
{
	bool insert= true;
	for (auto& i : issue_map)
	{
		Issue& issue = i.second;
		insert = insert && add_issue(issue);
	}

	return insert;
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
	constexpr const char* SELECT_ISSUE = "SELECT id, subject, state FROM issues";
	QSqlQuery query;
	if (!query.exec(QString(SELECT_ISSUE)))
		return map;

	while (query.next())
	{
		Issue iss;
		iss.set_id(query.value(0).toInt());
		iss.set_subject(query.value(1).toString());
		iss.set_state(static_cast<Issue::State>(query.value(2).toInt()));
		TimesliceVector timeslices = get_issue_timeslices(iss.get_id());
		for (auto& ts : timeslices)
			iss.add_timeslice(ts.second);
		map[iss.get_id()] = iss;
	}

	return map;
}

Issue DatabaseManager::get_issue_by_id(Issue::Id id)
{
	constexpr const char* SELECT_ISSUE = "SELECT subject, state"
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
	TimesliceVector timeslices = get_issue_timeslices(iss.get_id());
	for (auto& ts : timeslices)
		iss.add_timeslice(ts.second);

	return iss;
}

TimesliceVector DatabaseManager::get_issue_timeslices(Issue::Id id)
{
	constexpr const char* SELECT_TIMESLICE = "SELECT id, start, end, applied_to_redmine "
										 " FROM timeslice WHERE issue_id = %1";
	QSqlQuery query;
	if (!query.exec(QString(SELECT_TIMESLICE).arg(id)))
		return TimesliceVector();

	TimesliceVector vec;

	while (query.next())
	{
		Timeslice ts;
		ts.id = query.value(0).toInt();
		ts.start = QDateTime::fromString(query.value(1).toString());
		ts.end = QDateTime::fromString(query.value(2).toString());
		ts.applied_to_redmine = query.value(3).toBool();
		vec.insert(std::make_pair(ts.id, ts));
	}

	return vec;
}

bool DatabaseManager::add_category(int id, const QString& cat_text)
{
	QString insert_category("INSERT INTO categories VALUES(%1, '%2')");
	insert_category = insert_category.arg(id).arg(cat_text);

	return execute_query(insert_category);
}

std::map<int, QString> DatabaseManager::all_categories()
{
	std::map<int, QString> categories;
	QSqlQuery query;
	if (!query.exec(QString("SELECT id, cat_text FROM categories")))
		return categories;

	while (query.next())
		categories.insert(std::make_pair(query.value(0).toInt(), query.value(1).toString()));

	return categories;

}

bool DatabaseManager::remove_category(int id)
{
	QString query_string = QString("DELETE FROM categories WHERE id = %1").arg(id);
	return execute_query(query_string);
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
