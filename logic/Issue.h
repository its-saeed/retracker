#ifndef ISSUE_H
#define ISSUE_H

#include <chrono>
#include <QString>
#include <map>
#include "IssueTimeTracker.h"

class Issue
{
public:
	typedef uint32_t Id;
	enum State
	{
		NEW = 0,
		DOING,
		RETURNED,
		QUALITY_CHECK,
		UNKNOWN = -1
	};

	Issue();
	Issue(Id id, const QString& subject, State state);
	Issue(const Issue&) = default;
	quint32 get_id() const;
	void set_id(const quint32& value);

	QString get_subject() const;
	void set_subject(const QString& value);

	void set_total_spent_time(const std::chrono::minutes& min);
	void add_duration(const std::chrono::minutes& min);
	bool add_applied_duration(const std::chrono::minutes& min);
	void set_total_applied_to_redmine_time(const std::chrono::minutes& min);

	std::chrono::minutes total_duration() const;
	std::chrono::minutes total_applied_duration() const;
	std::chrono::minutes total_unapplied_duration() const;

	QString total_duration_string() const;
	QString total_applied_duration_string() const;
	QString total_unapplied_duration_string() const;
	static Issue::State state_from_string(const QString& state_string);
	State get_state() const;
	void set_state(State s);

	bool operator==(const Issue& other) const;

private:
	Id id;
	QString subject;
	State state;
	IssueTimeManager manager;
};

typedef std::map<Issue::Id, Issue> IssueMap;

#endif // ISSUE_H
