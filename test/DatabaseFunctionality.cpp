#include <gtest/gtest.h>
#include <chrono>
#include "DatabaseManager.h"

using namespace testing;

TEST(DatabaseFunctionality, add_issue_and_grabbing_it)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));
}
