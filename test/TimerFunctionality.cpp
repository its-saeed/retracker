#include <gtest/gtest.h>
#include <chrono>
#include "Timer.h"

using namespace testing;

TEST(TimerFunctionality, default_constructed_timer_should_return_zero_as_get_seconds)
{
	StopWatch timer;

	EXPECT_EQ(timer.get_seconds(), std::chrono::seconds{0});
}

TEST(TimerFunctionality, when_init_timer_get_seconds_should_return_inital_value)
{
	const std::chrono::seconds ten_seconds{10};
	StopWatch timer;
	timer.init(ten_seconds);

	EXPECT_EQ(timer.get_seconds(), ten_seconds);
}

TEST(TimerFunctionality, ticking_timer_2_times_with_10_as_initial_should_return_12_as_get_seconds)
{
	const std::chrono::seconds ten_seconds{10};
	StopWatch timer;
	timer.init(ten_seconds);
	timer.tick();
	timer.tick();

	EXPECT_EQ(timer.get_seconds(), std::chrono::seconds{12});
}

TEST(TimerFunctionality, reseting_timer_should_set_its_seconds_to_zero)
{
	const std::chrono::seconds ten_seconds{10};
	StopWatch timer;
	timer.init(ten_seconds);
	timer.tick();
	timer.tick();
	timer.reset();

	EXPECT_EQ(timer.get_seconds(), std::chrono::seconds{0});
}

TEST(TimerFunctionality, converting_timer_to_string_should_return_appropriate_string)
{
	const std::chrono::seconds ten_seconds{70};
	StopWatch timer;
	timer.init(ten_seconds);

	EXPECT_EQ(timer.to_string(), "00:01:10");
}

TEST(TimerFunctionality, converting_timer_to_string_should_return_appropriate_string_when_time_is_1_hour)
{
	const std::chrono::seconds ten_seconds = std::chrono::hours{1};
	StopWatch timer;
	timer.init(ten_seconds);

	EXPECT_EQ(timer.to_string(), "01:00:00");
}

TEST(TimerFunctionality, converting_timer_to_string_should_return_appropriate_string_when_time_is_1_minute)
{
	const std::chrono::seconds ten_seconds = std::chrono::minutes{1};
	StopWatch timer;
	timer.init(ten_seconds);

	EXPECT_EQ(timer.to_string(), "00:01:00");
}
