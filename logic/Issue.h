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
		NEW,
		DOING,
		RETURNED,
		QUALITY_CHECK,
		UNKNOWN
	};

	Issue();
	Issue(Id id, const QString& subject, State state);
	quint32 get_id() const;
	void set_id(const quint32& value);

	QString get_subject() const;
	void set_subject(const QString& value);

	void add_duration(const std::chrono::minutes& min);
	bool add_applied_duration(const std::chrono::minutes& min);

	std::chrono::minutes total_duration() const;
	std::chrono::minutes total_applied_duration() const;
	std::chrono::minutes total_unapplied_duration() const;

	QString total_duration_string() const;
	QString total_applied_duration_string() const;
	QString total_unapplied_duration_string() const;
	static Issue::State state_from_string(const QString& state_string);
	State get_state() const;

private:
	Id id;
	QString subject;
	State state;
	IssueTimeManager manager;
};

typedef std::map<Issue::Id, Issue> IssueMap;

#endif // ISSUE_H
