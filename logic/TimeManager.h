#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <chrono>

class TimeManager
{
public:
	TimeManager();
	void add_duration(const std::chrono::minutes& min);
	std::chrono::minutes get_total_duration() const;

	bool add_applied_duration(const std::chrono::minutes& min);
	std::chrono::minutes get_total_applied_duration() const;
	std::chrono::minutes get_total_unapplied_duration() const;

private:
	std::chrono::minutes total_duration;
	std::chrono::minutes total_applied_duration;
};

#endif // TIMEMANAGER_H
