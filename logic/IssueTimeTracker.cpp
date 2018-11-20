#include "IssueTimeTracker.h"
#include "commons.h"

IssueTimeManager::IssueTimeManager()
: total_useful_duration{0}
, total_applied_to_peygir_duration{0}
{

}

void IssueTimeManager::add_duration(const std::chrono::minutes& min)
{
	total_useful_duration += min;
}

void IssueTimeManager::set_total_spent_time(const std::chrono::minutes& min)
{
	total_useful_duration = min;
}

std::chrono::minutes IssueTimeManager::get_total_duration() const
{
	return total_useful_duration;
}

QString IssueTimeManager::total_duration_string() const
{
	return QString::fromStdString(commons::to_string(total_useful_duration));
}

bool IssueTimeManager::add_applied_duration(const std::chrono::minutes& min)
{
	if (total_applied_to_peygir_duration + min > total_useful_duration)
		return false;

	total_applied_to_peygir_duration += min;
	return true;
}

void IssueTimeManager::set_total_applied_to_redmine_time(const std::chrono::minutes& min)
{
	total_applied_to_peygir_duration = min;
}

std::chrono::minutes IssueTimeManager::get_total_applied_duration() const
{
	return total_applied_to_peygir_duration;
}

std::chrono::minutes IssueTimeManager::get_total_unapplied_duration() const
{
	return total_useful_duration - total_applied_to_peygir_duration;
}

QString IssueTimeManager::total_applied_duration_string() const
{
	return QString::fromStdString(commons::to_string(get_total_applied_duration()));
}

QString IssueTimeManager::total_unapplied_duration_string() const
{
	return QString::fromStdString(commons::to_string(get_total_unapplied_duration()));
}
