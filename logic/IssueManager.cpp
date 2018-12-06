#include "IssueManager.h"

#include <QFile>
#include <QDebug>

#include "ProccessRunner.h"
#include "DatabaseManager.h"

IssueManager::IssueManager(QObject* parent)
: QObject(parent)
, selected_issue_id(INVALID_ISSUE_ID)		// Invalid issue id, i.e. no issue is selected at first
{

}

bool IssueManager::issue_exists(Issue::Id id) const
{
	for (const auto& i : issues)
		qDebug() << "S: " << id << i.first << i.second.get_subject();
	return issues.find(id) != issues.end();
}

bool IssueManager::add_issue(Issue::Id id, QString subject, Issue::State state)
{
	return add_issue({id, subject, state});
}

bool IssueManager::add_issue(const Issue& issue)
{
	if (issue_exists(issue.get_id()))
	{
		last_error = "Trying to add a issue which already exists";
		return false;
	}

	issues[issue.get_id()] = issue;
	emit issue_added(issue.get_id());
	return true;
}

Issue& IssueManager::get_issue_by_id(Issue::Id id)
{
	static Issue default_constructed;
	if (!issue_exists(id))
		return default_constructed;

	return issues[id];
}

const Issue& IssueManager::get_issue_by_id(Issue::Id id) const
{
	static Issue default_constructed;
	if (!issue_exists(id))
		return default_constructed;

	return issues.at(id);
}

std::chrono::seconds IssueManager::get_total_useful_durations() const
{
	std::chrono::seconds total_duration{};
	for (const auto& i : issues)
	{
		const Issue& issue = i.second;
		total_duration += issue.get_total_duration();
	}

	return total_duration;
}

std::chrono::seconds IssueManager::get_total_useful_durations(Issue::Id id) const
{
	return get_issue_by_id(id).get_total_duration();
}

std::chrono::seconds IssueManager::get_total_useful_durations(const QDate& date) const
{
	std::chrono::seconds total_duration{};
	for (const auto& i : issues)
	{
		const Issue& issue = i.second;
		total_duration += issue.get_total_duration(date);
	}

	return total_duration;
}

std::chrono::seconds IssueManager::get_duration(Issue::Id id, const QDate& date) const
{
	return get_issue_by_id(id).get_duration(date);
}

int IssueManager::add_timeslice(Issue::Id id, const Timeslice& timeslice)
{
	Timeslice ts = timeslice;
	int tsid = DatabaseManager::instance().add_time_slice(selected_issue_id, timeslice);

	if (tsid == -1)
		return -1;

	ts.id = tsid;

	get_issue_by_id(id).add_timeslice(ts);
	issue_updated(id);

	return tsid;
}

bool IssueManager::update_timeslice(Issue::Id id, const Timeslice& ts)
{
	if (!issue_exists(id))
		return false;

	if (!get_issue_by_id(id).update_timeslice(ts))
		return false;

	issue_updated(id);

	return true;
}

bool IssueManager::delete_timeslice(Issue::Id id, int timeslice_id)
{
	if (!issue_exists(id))
		return false;

	if (!get_issue_by_id(id).delete_timeslice(timeslice_id))
		return false;

	issue_updated(id);

	return true;
}

void IssueManager::add_timeslice_for_selected_issue(const Timeslice& timeslice)
{
	if (selected_issue_id == INVALID_ISSUE_ID)
		return;

	add_timeslice(selected_issue_id, timeslice);
}

bool IssueManager::load_from_file(const QString& path)
{
	constexpr int SECTION_COUNT = 3;
	constexpr size_t ID_INDEX = 0;
	constexpr size_t SUBJECT_INDEX = 1;
	constexpr size_t STATUS_INDEX = 2;

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		last_error = "Cant open " + path + file.errorString();
		return false;
	}

	while (!file.atEnd())
	{
		QString line = file.readLine();
		QStringList parts = line.split(",");
		if (parts.size() < SECTION_COUNT)
			continue;

		Issue issue(parts.at(ID_INDEX).toInt(), parts.at(SUBJECT_INDEX), Issue::state_from_string(parts.at(STATUS_INDEX)));
		add_issue(issue);
	}

	return true;
}

void IssueManager::set_issues(IssueMap&& issue_map)
{
	issues = issue_map;
	emit issue_map_updated();
}

const IssueMap& IssueManager::get_issues() const
{
	return issues;
}

IssueMap& IssueManager::get_issues()
{
	return issues;
}

const QString&IssueManager::get_last_error() const
{
	return last_error;
}

void IssueManager::set_selected_issue_id(Issue::Id id)
{
	selected_issue_id = id;
}

const Issue& IssueManager::get_selected_issue() const
{
	static Issue issue;
	if (selected_issue_id == INVALID_ISSUE_ID)
		return issue;

	return get_issue_by_id(selected_issue_id);
}

bool IssueManager::load_issue_from_peygir(int issue_id, const QString& username, const QString& pass)
{
	ProccessRunner process("scripts/find_issue.py", QStringList() << username << pass << QString::number(issue_id));
	process.start_and_wait();

	// TOOD: create a logger
	//ui->pte_logs->appendPlainText(process.readAllStandardError());

	if (!load_from_file(".issues.txt"))
		return false;

	return true;
}

bool IssueManager::load_issues_from_peygir(const QString& username, const QString& pass)
{
	ProccessRunner process("scripts/red.py", QStringList() << username << pass);
	process.start_and_wait();
	// TODO, create a logger
	//ui->pte_logs->appendPlainText(process.readAllStandardError());
	if (!load_from_file("scripts/.issues.txt"))
		return false;

	return true;
}
