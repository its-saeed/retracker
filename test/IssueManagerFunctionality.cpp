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
	manager.add_duration(1, std::chrono::minutes(10));
	manager.add_duration(2, std::chrono::minutes(15));
	EXPECT_EQ(manager.get_total_useful_durations(), std::chrono::minutes(25));
}

TEST(IssueManagerFunctionality, testing_getting_useful_time_by_issue_id)
{
	IssueManager manager;
	EXPECT_TRUE(manager.add_issue(1, "Salam", Issue::NEW));
	EXPECT_TRUE(manager.add_issue(2, "Salam", Issue::NEW));
	manager.add_duration(1, std::chrono::minutes(10));
	manager.add_duration(2, std::chrono::minutes(15));
	EXPECT_EQ(manager.get_duration(1), std::chrono::minutes(10));
}
