#include <gtest/gtest.h>
#include <chrono>
#include "TimeManager.h"

using namespace testing;

TEST(TimeManagerFunctionalityTest, when_adding_time_to_time_manager_total_time_should_be_sum_of_new_and_old)
{
	TimeManager manager;
	std::chrono::minutes twenty{20};

	manager.add_duration(twenty);

	EXPECT_EQ(manager.get_total_duration(), twenty);

	manager.add_duration(twenty);
	EXPECT_EQ(manager.get_total_duration(), std::chrono::minutes{40});
}

TEST(TimeManagerFunctionalityTest, unapplied_time_to_redmine_is_equal_to_total_time_minus_applied_time)
{
	TimeManager manager;
	std::chrono::minutes eight_min{8};
	std::chrono::minutes twelve_min{12};
	std::chrono::minutes twenty_min{20};

	manager.add_duration(twenty_min);
	EXPECT_TRUE(manager.add_applied_duration(eight_min));
	EXPECT_EQ(manager.get_total_applied_duration(), eight_min);
	EXPECT_EQ(manager.get_total_unapplied_duration(), twelve_min);

	EXPECT_TRUE(manager.add_applied_duration(twelve_min));
	EXPECT_EQ(manager.get_total_applied_duration(), twenty_min);
	EXPECT_EQ(manager.get_total_unapplied_duration(), std::chrono::minutes{0});

	EXPECT_FALSE(manager.add_applied_duration(twelve_min));
}
