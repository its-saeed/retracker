#ifndef STATISTICS_H
#define STATISTICS_H

#include <QTime>
#include "IssueTimeTracker.h"

class Statistics
{
public:
	Statistics();
	QString day_start_string() const;
	void set_day_start(const QTime time);
	void add_useful_duration(const std::chrono::minutes& duration);
	QString useful_duration_string() const;
	std::chrono::minutes today_duration(const QTime& current_time) const;
	QString today_duration_string(const QTime& current_time) const;
	float today_efficiency(const QTime& current_time) const;

private:
	QTime day_start;
	IssueTimeManager total_useful_time_tracker;
};

#endif // STATISTICS_H
