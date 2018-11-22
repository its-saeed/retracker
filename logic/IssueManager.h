#ifndef ISSUEMANAGER_H
#define ISSUEMANAGER_H

#include <QObject>
#include <map>

#include "Issue.h"

class IssueManager : public QObject
{
	Q_OBJECT
public:
	IssueManager(QObject* parent = nullptr);
	bool issue_exists(Issue::Id id) const;
	bool add_issue(Issue::Id id, QString subject, Issue::State);
	bool add_issue(const Issue& issue);
	Issue& get_issue_by_id(Issue::Id id);
	const Issue& get_issue_by_id(Issue::Id id) const;
	std::chrono::minutes get_total_useful_durations() const;
	std::chrono::minutes get_duration(Issue::Id id) const;
	void add_duration(Issue::Id id, const std::chrono::minutes& mins);
	/// Add duration for currently selected issue
	void add_timeslice_for_selected_issue(const std::chrono::minutes& mins);
	bool add_applied_duration(Issue::Id id, const std::chrono::minutes& mins);
	bool load_from_file(const QString& path);
	void set_issues(IssueMap&& issue_map);
	const IssueMap& get_issues() const;
	const QString& get_last_error() const;
	void set_selected_issue_id(Issue::Id id);
	const Issue& get_selected_issue() const;

	bool load_issue_from_peygir(int issue_id, const QString& username, const QString& pass);
	bool load_issues_from_peygir(const QString& username, const QString& pass);
signals:
	void issue_updated(Issue::Id id);
	void issue_added(Issue::Id id);
	void issue_map_updated();

private:
	static constexpr Issue::Id INVALID_ISSUE_ID = 0;

	IssueMap issues;
	QString last_error;
	Issue::Id selected_issue_id;
};

#endif // ISSUEMANAGER_H
