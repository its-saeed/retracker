#ifndef ISSUEMANAGER_H
#define ISSUEMANAGER_H

#include <map>

#include "Issue.h"

class IssueManager
{
public:
	bool issue_exists(Issue::Id id) const;
	bool add_issue(Issue::Id id, QString subject, Issue::State);
	bool add_issue(const Issue& issue);
	Issue& get_issue_by_id(Issue::Id id);
	const Issue& get_issue_by_id(Issue::Id id) const;
	std::chrono::minutes get_total_useful_durations() const;
	std::chrono::minutes get_duration(Issue::Id id) const;
	void add_duration(Issue::Id id, const std::chrono::minutes& mins);
	bool add_applied_duration(Issue::Id id, const std::chrono::minutes& mins);

private:
	IssueMap issues;
};

#endif // ISSUEMANAGER_H
