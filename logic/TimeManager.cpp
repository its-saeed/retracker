#include "TimeManager.h"

TimeManager::TimeManager()
: total_duration{0}
, total_applied_duration{0}
{

}

void TimeManager::add_duration(const std::chrono::minutes& min)
{
	total_duration += min;
}

std::chrono::minutes TimeManager::get_total_duration() const
{
	return total_duration;
}

bool TimeManager::add_applied_duration(const std::chrono::minutes& min)
{
	if (total_applied_duration + min > total_duration)
		return false;

	total_applied_duration += min;
	return true;
}

std::chrono::minutes TimeManager::get_total_applied_duration() const
{
	return total_applied_duration;
}

std::chrono::minutes TimeManager::get_total_unapplied_duration() const
{
	return total_duration - total_applied_duration;
}
