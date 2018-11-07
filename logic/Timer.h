#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>

class StopWatch
{
public:
	StopWatch(const std::chrono::seconds& seconds = std::chrono::seconds{0});
	void init(const std::chrono::seconds& seconds);
	std::chrono::seconds get_seconds() const;
	std::chrono::minutes get_minutes() const;
	uint64_t get_seconds_count() const;
	std::string to_string() const;
	void tick();
	void reset();

private:
	std::chrono::seconds duration;
};

#endif // TIMER_H
