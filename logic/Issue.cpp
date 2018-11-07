#include "Issue.h"

Issue::Issue()
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
