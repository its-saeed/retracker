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

TEST(StatisticsFunctionality, when_useful_time_equals_to_day_duration_efficiency_should_be_100)
{
	QTime start_time = QTime::currentTime();
	QTime current_time = start_time.addSecs(1200);

	Statistics stats;
	stats.set_day_start(start_time);
	stats.add_useful_duration(std::chrono::minutes(20));

	EXPECT_NEAR(stats.today_efficiency(current_time), 100.0, 0.01);
}

TEST(StatisticsFunctionality, when_useful_time_equals_to_80_percent_of_day_duration_efficiency_should_be_80)
{
	QTime start_time = QTime::currentTime();
	QTime current_time = start_time.addSecs(1200);

	Statistics stats;
	stats.set_day_start(start_time);
	stats.add_useful_duration(std::chrono::minutes(16));

	EXPECT_NEAR(stats.today_efficiency(current_time), 80.0, 0.01);
}
