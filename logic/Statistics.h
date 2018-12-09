#ifndef STATISTICS_H
#define STATISTICS_H

#include <QTime>
#include "IssueTimeTracker.h"

class Statistics
{
public:
	Statistics();
	QString day_start_string() const;
	void set_day_start(const QTime& time);
	std::chrono::seconds today_duration(const QTime& current_time) const;
	QString today_duration_string(const QTime& current_time) const;
	float today_efficiency(const QTime& current_time, std::chrono::seconds useful_seconds) const;

private:
	QTime day_start;
};

#endif // STATISTICS_H
