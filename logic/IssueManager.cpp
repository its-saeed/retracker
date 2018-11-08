#include "IssueManager.h"

#include <QFile>

bool IssueManager::issue_exists(Issue::Id id) const
{
	return issues.find(id) != issues.end();
}

bool IssueManager::add_issue(Issue::Id id, QString subject, Issue::State state)
{
	return add_issue({id, subject, state});
}

bool IssueManager::add_issue(const Issue& issue)
{
	if (issue_exists(issue.get_id()))
		return false;

	issues[issue.get_id()] = issue;
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

std::chrono::minutes IssueManager::get_total_useful_durations() const
{
	std::chrono::minutes total_duration{};
	for (const auto& i : issues)
	{
		const Issue& issue = i.second;
		total_duration += issue.total_duration();
	}

	return total_duration;
}

std::chrono::minutes IssueManager::get_duration(Issue::Id id) const
{
	return get_issue_by_id(id).total_duration();
}

void IssueManager::add_duration(Issue::Id id, const std::chrono::minutes& mins)
{
	get_issue_by_id(id).add_duration(mins);
}

bool IssueManager::add_applied_duration(Issue::Id id, const std::chrono::minutes& mins)
{
	return get_issue_by_id(id).add_applied_duration(mins);
}

bool IssueManager::load_from_file(const QString& path)
{
	constexpr size_t SECTION_COUNT = 3;
	constexpr size_t ID_INDEX = 0;
	constexpr size_t SUBJECT_INDEX = 1;
	constexpr size_t STATUS_INDEX = 2;

	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;

	while (!file.atEnd())
	{
		QString line = file.readLine();
		QStringList parts = line.split(",");
		if (parts.size() < SECTION_COUNT)
			return false;

		Issue issue(parts.at(ID_INDEX).toInt(), parts.at(SUBJECT_INDEX), Issue::state_from_string(parts.at(STATUS_INDEX)));
		add_issue(issue);
	}

	return true;
}

const IssueMap& IssueManager::get_issues() const
{
	return issues;
}

