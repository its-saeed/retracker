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
