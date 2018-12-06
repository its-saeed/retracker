#include <gtest/gtest.h>
#include <chrono>
#include <QDateTime>
#include <QDateTime>
#include "DatabaseManager.h"

using namespace testing;

TEST(DatabaseFunctionality, add_issue_and_grabbing_it)
{
	DatabaseManager db;
	ASSERT_TRUE(db.open_database(":memory:"));

	Issue issue(1, "salam", Issue::State::NEW);
	QDateTime d = QDateTime::currentDateTime();
	Timeslice t{1, d, d.addSecs(10), false};
	issue.add_timeslice(t);
	ASSERT_TRUE(db.add_issue(issue));

	Issue returned_by_db = db.get_issue_by_id(1);
	TimesliceVector ts = db.get_issue_timeslices(returned_by_db.get_id());

	EXPECT_EQ(returned_by_db.get_id(), issue.get_id());
	EXPECT_EQ(returned_by_db.get_subject(), issue.get_subject());
	EXPECT_EQ(returned_by_db.get_state(), issue.get_state());
	//EXPECT_EQ(returned_by_db.get_timeslices(), issue.get_timeslices());
	EXPECT_EQ(ts.size(), 1);

	for (auto const& i : ts)
		EXPECT_EQ(t, i.second);
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

TEST(DatabaseFunctionality, adding_timeslice_for_non_existant_issue_should_return_false)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	//Issue issue1(1, "salam", Issue::State::NEW);
	//EXPECT_TRUE(db.add_issue(issue1));

	//EXPECT_FALSE(db.add_time_slice(1, QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(10), false));
	Issue issue2 = db.get_issue_by_id(1);

}

TEST(DatabaseFunctionality, adding_two_category_with_same_id_should_return_false)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	EXPECT_TRUE(db.add_category(1, "salam"));
	EXPECT_FALSE(db.add_category(1, "salam"));
}

TEST(DatabaseFunctionality, adding_and_grabbing_category)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));
	db.add_category(1, "salam");

	auto cats = db.all_categories();

	EXPECT_EQ(cats.size(), 1);
	EXPECT_EQ(cats[1], QString("salam"));
}

TEST(DatabaseFunctionality, add_two_timeslices_should_return_1_2_as_their_ids)
{
	DatabaseManager db;
	EXPECT_TRUE(db.open_database(":memory:"));

	int id = db.add_time_slice(1, {1, QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(10), false});
	EXPECT_EQ(id, 1);

	id = db.add_time_slice(1, {1, QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(10), false});
	EXPECT_EQ(id, 2);
}

