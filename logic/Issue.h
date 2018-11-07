#ifndef ISSUE_H
#define ISSUE_H

#include <chrono>
#include <QString>
#include "IssueTimeTracker.h"

class Issue
{
public:
	Issue();
	quint32 get_id() const;
	void set_id(const quint32& value);

	QString get_subject() const;
	void set_subject(const QString& value);

	void add_duration(const std::chrono::minutes& min);
	bool add_applied_duration(const std::chrono::minutes& min);

private:
	quint32 id;
	QString subject;
	quint8 state;
	IssueTimeManager manager;
};

#endif // ISSUE_H
