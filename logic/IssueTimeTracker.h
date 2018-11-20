#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <QString>
#include <chrono>

class IssueTimeManager
{
public:
	IssueTimeManager();
	void add_duration(const std::chrono::minutes& min);
	void set_total_spent_time(const std::chrono::minutes& min);
	std::chrono::minutes get_total_duration() const;
	QString total_duration_string() const;

	bool add_applied_duration(const std::chrono::minutes& min);
	void set_total_applied_to_redmine_time(const std::chrono::minutes& min);
	std::chrono::minutes get_total_applied_duration() const;
	std::chrono::minutes get_total_unapplied_duration() const;
	QString total_applied_duration_string() const;
	QString total_unapplied_duration_string() const;

private:
	std::chrono::minutes total_useful_duration;
	std::chrono::minutes total_applied_to_peygir_duration;
};

#endif // TIMEMANAGER_H
