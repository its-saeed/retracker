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

	void set_selected_issue_id(Issue::Id id);
	const Issue& get_selected_issue() const;
	bool issue_exists(Issue::Id id) const;
	void set_issues(IssueMap&& issue_map);
	const IssueMap& get_issues() const;
	IssueMap& get_issues();
	bool add_issue(Issue::Id id, QString subject, Issue::State);
	bool add_issue(const Issue& issue);
	Issue& get_issue_by_id(Issue::Id id);
	const Issue& get_issue_by_id(Issue::Id id) const;

	std::chrono::seconds get_total_useful_durations() const;
	std::chrono::seconds get_total_useful_durations(Issue::Id id) const;
	std::chrono::seconds get_total_useful_durations(const QDate& date) const;
	std::chrono::seconds get_duration(Issue::Id id, const QDate& date) const;

	/// Add duration for currently selected issue
	void add_timeslice_for_selected_issue(const Timeslice& timeslice);
	int add_timeslice(Issue::Id id, const Timeslice& timeslice);
	bool update_timeslice(Issue::Id id, const Timeslice& ts);
	bool delete_timeslice(Issue::Id id, int timeslice_id);

	const QString& get_last_error() const;

	bool load_issue_from_peygir(int issue_id, const QString& username, const QString& pass);
	bool load_issues_from_peygir(const QString& username, const QString& pass);
	bool load_from_file(const QString& path);
	bool set_issue_timeslices_applied_to_redmine(Issue::Id id);
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
