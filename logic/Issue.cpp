#include "Issue.h"
#include "commons.h"

Issue::Issue()
: id(0)
, state(UNKNOWN)
{

}

Issue::Issue(Issue::Id id, const QString& subject, Issue::State state)
: id(id)
, subject(subject)
, state(state)
{
}

quint32 Issue::get_id() const
{
    return id;
}

void Issue::set_id(const quint32& value)
{
    id = value;
}

QString Issue::get_subject() const
{
	return subject;
}

void Issue::set_subject(const QString& value)
{
	subject = value;
}

Issue::State Issue::state_from_string(const QString& state_string)
{
	if (state_string.trimmed() == "جدید")
		return Issue::State::NEW;

	if (state_string.trimmed() == "در حال انجام")
		return Issue::State::DOING;

	if (state_string.trimmed() == "بازخورد")
		return Issue::State::RETURNED;

	if (state_string.trimmed() == "بررسی کیفیت")
		return Issue::State::QUALITY_CHECK;

	return State::UNKNOWN;
}

Issue::State Issue::get_state() const
{
	return state;
}

void Issue::set_state(Issue::State s)
{
	state = s;
}

bool Issue::add_timeslice(const Timeslice& timeslice)
{
	return timeslice_manager.add_timeslice(timeslice);
}

bool Issue::update_timeslice(const Timeslice& ts)
{
	return timeslice_manager.update_timeslice(ts);
}

bool Issue::delete_timeslice(int id)
{
	return timeslice_manager.delete_timeslice(id);
}

void Issue::set_timeslices_applied_to_redmine()
{
	timeslice_manager.set_timeslices_applied_to_redmine();
}

std::chrono::seconds Issue::get_duration(const QDate& date) const
{
	return timeslice_manager.get_duration(date);
}

std::chrono::seconds Issue::get_duration(const QDate& date, bool applied) const
{
	return timeslice_manager.get_duration(date, applied);
}

std::chrono::seconds Issue::get_total_duration() const
{
	return timeslice_manager.get_total_duration();
}

std::chrono::seconds Issue::get_total_duration(bool applied) const
{
	return timeslice_manager.get_total_duration(applied);
}

std::chrono::seconds Issue::get_total_duration(const QDate& date) const
{
	return timeslice_manager.get_total_duration(date);
}

QString Issue::get_duration_string(const QDate& date) const
{
	return QString::fromStdString(commons::to_string(get_duration(date)));
}

QString Issue::get_duration_string(const QDate& date, bool applied) const
{
	return QString::fromStdString(commons::to_string(get_duration(date, applied)));
}

QString Issue::get_total_duration_string() const
{
	return QString::fromStdString(commons::to_string(get_total_duration()));
}

QString Issue::get_total_duration_string(bool applied) const
{
	return QString::fromStdString(commons::to_string(get_total_duration(applied)));
}

TimesliceVector Issue::get_timeslices() const
{
	return timeslice_manager.get_timeslices();
}

bool Issue::operator==(const Issue& other) const
{
	return (other.get_id() == get_id()
			&& other.get_subject() == get_subject()
			&& other.get_state() == get_state()
			&& other.timeslice_manager.get_timeslices() == timeslice_manager.get_timeslices());
}

bool TimesliceManager::add_timeslice(const Timeslice& timeslice)
{
	timeslices.insert(std::make_pair(timeslice.id, timeslice));
	return true;
}

bool TimesliceManager::update_timeslice(const Timeslice& timeslice)
{
	if (timeslices.find(timeslice.id) == timeslices.end())
		return false;

	timeslices[timeslice.id] = timeslice;

	return true;
}

bool TimesliceManager::delete_timeslice(int id)
{
	if (timeslices.find(id) == timeslices.end())
		return false;

	timeslices.erase(id);

	return true;
}

void TimesliceManager::set_timeslices_applied_to_redmine()
{
	for (auto& i : timeslices)
		i.second.applied_to_redmine = true;
}

std::chrono::seconds TimesliceManager::get_duration(const QDate& date) const
{
	std::chrono::seconds duration{0};
	for (const auto& ts : timeslices)
	{
		if (ts.second.start.date() == date)
			duration += ts.second.get_duration();
	}

	return duration;
}

std::chrono::seconds TimesliceManager::get_duration(const QDate& date, bool applied) const
{
	std::chrono::seconds duration{0};
	for (const auto& ts : timeslices)
	{
		if (ts.second.start.date() == date && ts.second.applied_to_redmine == applied)
			duration += ts.second.get_duration();
	}

	return duration;
}

std::chrono::seconds TimesliceManager::get_total_duration() const
{
	std::chrono::seconds duration{0};
	for (const auto& ts : timeslices)
		duration += ts.second.get_duration();

	return duration;
}

std::chrono::seconds TimesliceManager::get_total_duration(bool applied) const
{
	std::chrono::seconds duration{0};
	for (const auto& ts : timeslices)
		if (ts.second.applied_to_redmine == applied)
			duration += ts.second.get_duration();

	return duration;
}

std::chrono::seconds TimesliceManager::get_total_duration(const QDate& date) const
{
	std::chrono::seconds duration{0};
	for (const auto& ts : timeslices)
		if (ts.second.start.date() == date)			// Assumed that timeslice start and end is in same day
			duration += ts.second.get_duration();

	return duration;
}

TimesliceVector TimesliceManager::get_timeslices() const
{
	return timeslices;
}
