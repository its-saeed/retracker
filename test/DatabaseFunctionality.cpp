#include <gtest/gtest.h>
#include <chrono>
#include "DatabaseManager.h"

using namespace testing;

TEST(DatabaseFunctionality, add_issue_and_grabbing_it)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	Issue issue(1, "salam", Issue::State::NEW);
	EXPECT_TRUE(db.add_issue(issue));

	Issue returned_by_db = db.get_issue_by_id(1);

	EXPECT_EQ(returned_by_db, issue);
}

TEST(DatabaseFunctionality, adding_two_issues_with_same_id_should_return_false)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	Issue issue(1, "salam", Issue::State::NEW);
	EXPECT_TRUE(db.add_issue(issue));
	EXPECT_FALSE(db.add_issue(issue));
}

TEST(DatabaseFunctionality, adding_two_issues_with_different_id_should_be_successful)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	Issue issue1(1, "salam", Issue::State::NEW);
	Issue issue2(2, "salam", Issue::State::NEW);
	EXPECT_TRUE(db.add_issue(issue1));
	EXPECT_TRUE(db.add_issue(issue2));

	IssueMap map = db.all_issues();
	EXPECT_EQ(map.size(), 2);
	EXPECT_EQ(map[1], issue1);
	EXPECT_EQ(map[2], issue2);
}

TEST(DatabaseFunctionality, update_issue_should_take_effect_in_database)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	Issue issue1(1, "salam", Issue::State::NEW);
	EXPECT_TRUE(db.add_issue(issue1));

	issue1.set_subject("keke");
	issue1.set_total_applied_to_redmine_time(std::chrono::minutes(20));
	issue1.set_total_spent_time(std::chrono::minutes(23));
	issue1.set_state(Issue::State::QUALITY_CHECK);

	EXPECT_TRUE(db.update_issue(issue1));
	Issue issue2 = db.get_issue_by_id(1);

	EXPECT_EQ(issue1, issue2);
}

TEST(DatabaseFunctionality, deleting_issue_should_delete_it_from_database)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	Issue issue1(1, "salam", Issue::State::NEW);
	EXPECT_TRUE(db.add_issue(issue1));

	EXPECT_TRUE(db.remove_issue(issue1.get_id()));
	Issue issue2 = db.get_issue_by_id(1);

	EXPECT_EQ(Issue(), issue2);
}
