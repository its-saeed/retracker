#include <gtest/gtest.h>
#include <chrono>
#include "Statistics.h"

using namespace testing;

TEST(StatisticsFunctionality, day_duration_test)
{
	QTime start_time = QTime::currentTime();
	QTime current_time = start_time.addSecs(120);

	Statistics stats;
	stats.set_day_start(start_time);

	EXPECT_EQ(stats.today_duration(current_time), std::chrono::minutes{2});
}
