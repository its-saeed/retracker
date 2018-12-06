#include "IssueManager.h"
#include <gtest/gtest.h>

using namespace testing;

TEST(IssueManagerFunctionality, getting_non_existing_issue_returns_default_constructed)
{
	IssueManager manager;
	Issue& issue = manager.get_issue_by_id(1111);

	EXPECT_EQ(issue.get_id(), 0);
}

TEST(IssueManagerFunctionality, getting_recently_added_issue_should_return_it)
{
	IssueManager manager;
	manager.add_issue(1, "Salam", Issue::NEW);
	Issue& issue = manager.get_issue_by_id(1);

	EXPECT_EQ(issue.get_id(), 1);
	EXPECT_EQ(issue.get_subject(), "Salam");
}

TEST(IssueManagerFunctionality, adding_previously_added_issue_should_return_false)
{
	IssueManager manager;
	EXPECT_TRUE(manager.add_issue(1, "Salam", Issue::NEW));
	EXPECT_FALSE(manager.add_issue(1, "Salam", Issue::NEW));
}

TEST(IssueManagerFunctionality, issue_manager_total_time_is_equal_to_sum_of_all_issues)
{
	IssueManager manager;
	EXPECT_TRUE(manager.add_issue(1, "Salam", Issue::NEW));
	EXPECT_TRUE(manager.add_issue(2, "Salam", Issue::NEW));
	
	QDateTime d = QDateTime::currentDateTime();
	manager.add_timeslice(1, {1, d, d.addSecs(10 * 60), false});
	manager.add_timeslice(2, {2, d, d.addSecs(15 * 60), false});
	EXPECT_EQ(manager.get_total_useful_durations(), std::chrono::minutes(25));
}

TEST(IssueManagerFunctionality, testing_getting_useful_time_by_issue_id)
{
	IssueManager manager;
	EXPECT_TRUE(manager.add_issue(1, "Salam", Issue::NEW));
	EXPECT_TRUE(manager.add_issue(2, "Salam", Issue::NEW));
	QDateTime d = QDateTime::currentDateTime();
	manager.add_timeslice(1, {1, d, d.addSecs(10 * 60), false});
	manager.add_timeslice(2, {2, d, d.addSecs(15 * 60), false});
	EXPECT_EQ(manager.get_total_useful_durations(1), std::chrono::minutes(10));
}

TEST(IssueFunctionality, testing_state_string_to_enum)
{
	EXPECT_EQ(Issue::state_from_string("جدید"), Issue::State::NEW);
	EXPECT_EQ(Issue::state_from_string("در حال انجام"), Issue::State::DOING);
	EXPECT_EQ(Issue::state_from_string("بازخورد"), Issue::State::RETURNED);
	EXPECT_EQ(Issue::state_from_string("بررسی کیفیت"), Issue::State::QUALITY_CHECK);
	EXPECT_EQ(Issue::state_from_string("کیفیت"), Issue::State::UNKNOWN);
}
