#include <gtest/gtest.h>
#include <chrono>
#include <QTime>
#include "commons.h"
#include "OnDestructionRunner.h"

using namespace testing;

TEST(CommonsFunctionality, converting_70_seconds_to_string)
{
	const std::chrono::seconds ten_seconds{70};

	EXPECT_EQ(commons::to_string(ten_seconds), "00:01:10");
}

TEST(CommonsFunctionality, converting_one_hour_to_seconds_and_then_string)
{
	const std::chrono::seconds one_hour_seconds = std::chrono::hours{1};

	EXPECT_EQ(commons::to_string(one_hour_seconds), "01:00:00");
}

TEST(CommonsFunctionality, converting_one_minute_to_seconds_and_then_string)
{
	const std::chrono::seconds one_min = std::chrono::minutes{1};

	EXPECT_EQ(commons::to_string(one_min), "00:01:00");
}

TEST(CommonsFunctionality, converting_70_minutes_to_string)
{
	const std::chrono::minutes seventy_minutes{70};

	EXPECT_EQ(commons::to_string(seventy_minutes), "01:10");
}

TEST(CommonsFunctionality, converting_one_hour_to_minutes_and_then_string)
{
	const std::chrono::minutes one_hour_minutes = std::chrono::hours{1};

	EXPECT_EQ(commons::to_string(one_hour_minutes), "01:00");
}

TEST(CommonsFunctionality, converting_one_minute_string)
{
	const std::chrono::minutes one_min{1};

	EXPECT_EQ(commons::to_string(one_min), "00:01");
}

TEST(CommonsFunctionality, get_today_when_previous_is_null_should_return_current_date)
{
	const QDateTime current = QDateTime::currentDateTime();
	EXPECT_EQ(commons::get_today(current), current);
}

TEST(CommonsFunctionality, get_today_should_return_current_date_if_both_prev_and_current_date_are_in_same_day_and_current_bigger_prev)
{
	const QDateTime current = QDateTime(QDate(2018, 10, 10), QTime(10, 30, 0));
	const QDateTime prev = QDateTime(QDate(2018, 10, 10), QTime(10, 00, 0));
	EXPECT_EQ(commons::get_today(current, prev), prev);
}

TEST(CommonsFunctionality, get_today_should_return_invalid_date_if_both_prev_and_current_date_are_in_same_day_and_current_smaller_prev)
{
	const QDateTime prev = QDateTime(QDate(2018, 10, 10), QTime(10, 30, 0));
	const QDateTime current = QDateTime(QDate(2018, 10, 10), QTime(10, 00, 0));
	EXPECT_EQ(commons::get_today(current, prev), QDateTime());
}

TEST(CommonsFunctionality, get_today_should_return_invalid_date_if_prev_and_current_date_are_not_in_same_date)
{
	const QDateTime prev = QDateTime(QDate(2018, 10, 15), QTime(10, 30, 0));
	const QDateTime current = QDateTime(QDate(2018, 10, 10), QTime(10, 30, 0));
	EXPECT_EQ(commons::get_today(current, prev), QDateTime());
}

TEST(CommonsFunctionality, testing_valid_time_to_double)
{
	EXPECT_NEAR(commons::time_to_double("3:30:00"), 3.5, 0.001);
}

TEST(CommonsFunctionality, invalid_delimiter_should_return_zero)
{
	EXPECT_NEAR(commons::time_to_double("3,30:00"), 0, 0.001);
}

TEST(CommonsFunctionality, invalid_time_should_return_zero)
{
	EXPECT_NEAR(commons::time_to_double("3:70:00"), 0, 0.001);
}

TEST(OnDestructionFunctionality, on_destruction_of_object_it_should_do_what_is_expected)
{
	int a = 10;
	{
		OnDestructionRunner dest([&a](){++a;});
	}

	EXPECT_EQ(a, 11);
}
