#include "IssueManager.h"

bool IssueManager::issue_exists(Issue::Id id) const
{
	return issues.find(id) != issues.end();
}

bool IssueManager::add_issue(Issue::Id id, QString subject, Issue::State state)
{
	add_issue({id, subject, state});
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
