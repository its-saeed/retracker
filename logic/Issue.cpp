#include "Issue.h"

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

void Issue::add_duration(const std::chrono::minutes& min)
{
	manager.add_duration(min);
}

bool Issue::add_applied_duration(const std::chrono::minutes& min)
{
	return manager.add_applied_duration(min);
}

std::chrono::minutes Issue::total_duration() const
{
	return manager.get_total_duration();
}

std::chrono::minutes Issue::total_applied_duration() const
{
	return manager.get_total_applied_duration();
}

std::chrono::minutes Issue::total_unapplied_duration() const
{
	return manager.get_total_unapplied_duration();
}

QString Issue::total_duration_string() const
{
	return manager.total_duration_string();
}

QString Issue::total_applied_duration_string() const
{
	return manager.total_applied_duration_string();
}

QString Issue::total_unapplied_duration_string() const
{
	return manager.total_unapplied_duration_string();
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
