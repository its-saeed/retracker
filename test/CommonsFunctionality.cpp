#include <gtest/gtest.h>
#include <chrono>
#include <QTime>
#include "commons.h"

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
