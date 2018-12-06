#ifndef ISSUE_H
#define ISSUE_H

#include <chrono>
#include <map>

#include <QString>
#include <QDateTime>

struct Timeslice
{
	int id;
	QDateTime start;
	QDateTime end;
	bool applied_to_redmine;

	std::chrono::seconds get_duration() const
	{
		return std::chrono::seconds{start.secsTo(end)};
	}

	bool operator==(const Timeslice& other) const
	{
		return id == other.id && start == other.start && end == other.end && applied_to_redmine == other.applied_to_redmine;
	}
};

typedef std::map<int, Timeslice> TimesliceVector;		// ID -> Timeslice

class TimesliceManager
{
public:
	bool add_timeslice(const Timeslice& timeslice);
	bool update_timeslice(const Timeslice& timeslice);
	bool delete_timeslice(int id);
	std::chrono::seconds get_duration(const QDate& date) const;
	std::chrono::seconds get_duration(const QDate& date, bool applied) const;
	std::chrono::seconds get_total_duration() const;
	std::chrono::seconds get_total_duration(bool applied) const;
	std::chrono::seconds get_total_duration(const QDate& date) const;
	TimesliceVector get_timeslices() const;
private:
	TimesliceVector timeslices;
};

class Issue
{
public:
	typedef uint32_t Id;
	enum State
	{
		NEW = 0,
		DOING,
		RETURNED,
		QUALITY_CHECK,
		UNKNOWN = -1
	};

	Issue();
	Issue(Id id, const QString& subject, State state);
	Issue(const Issue&) = default;
	quint32 get_id() const;
	void set_id(const quint32& value);

	QString get_subject() const;
	void set_subject(const QString& value);

	static Issue::State state_from_string(const QString& state_string);
	State get_state() const;
	void set_state(State s);

	bool add_timeslice(const Timeslice& timeslice);
	bool update_timeslice(const Timeslice& ts);
	bool delete_timeslice(int id);
	std::chrono::seconds get_duration(const QDate& date) const;
	std::chrono::seconds get_duration(const QDate& date, bool applied) const;
	std::chrono::seconds get_total_duration() const;
	std::chrono::seconds get_total_duration(bool applied) const;
	std::chrono::seconds get_total_duration(const QDate& date) const;

	QString get_duration_string(const QDate& date) const;
	QString get_duration_string(const QDate& date, bool applied) const;
	QString get_total_duration_string() const;
	QString get_total_duration_string(bool applied) const;

	TimesliceVector get_timeslices() const;

	bool operator==(const Issue& other) const;

private:
	Id id;
	QString subject;
	State state;
	TimesliceManager timeslice_manager;
};

typedef std::map<Issue::Id, Issue> IssueMap;

#endif // ISSUE_H
