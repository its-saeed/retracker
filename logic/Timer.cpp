#include "Timer.h"
#include "commons.h"

StopWatch::StopWatch(const std::chrono::seconds& seconds)
: duration(seconds)
{

}

void StopWatch::init(const std::chrono::seconds& seconds)
{
	duration = seconds;
}

std::chrono::seconds StopWatch::get_seconds() const
{
	return duration;
}

std::chrono::minutes StopWatch::get_minutes() const
{
	return std::chrono::duration_cast<std::chrono::minutes>(duration);
}

uint64_t StopWatch::get_seconds_count() const
{
	return duration.count();
}

std::string StopWatch::to_string() const
{
	return commons::to_string(duration);
}

void StopWatch::tick()
{
	duration += std::chrono::seconds{1};
}

void StopWatch::reset()
{
	duration = std::chrono::seconds{0};
}
