#include "Statistics.h"
#include "commons.h"

Statistics::Statistics()
{

}

QString Statistics::day_start_string() const
{
	return day_start.toString();
}

void Statistics::set_day_start(const QTime& time)
{
	day_start = time;
}

std::chrono::seconds Statistics::today_duration(const QTime& current_time) const
{
	int seconds = day_start.secsTo(current_time);
	return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::seconds{seconds});
}

QString Statistics::today_duration_string(const QTime& current_time) const
{
	return QString::fromStdString(commons::to_string(today_duration(current_time)));
}

float Statistics::today_efficiency(const QTime& current_time, std::chrono::seconds useful_seconds) const
{
	return 100 * (useful_seconds.count() / (float)today_duration(current_time).count());
}
