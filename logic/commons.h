#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <chrono>
#include <QDateTime>

namespace commons
{

inline std::string to_string(const std::chrono::minutes& minutes)
{
	std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
	std::chrono::minutes mins = minutes - hours;
	QTime time(hours.count(), mins.count());
	return time.toString("hh:mm").toStdString();
}

inline std::string to_string(const std::chrono::seconds& seconds)
{
	std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);
	std::chrono::minutes mins = std::chrono::duration_cast<std::chrono::minutes>(seconds - hours);
	std::chrono::seconds remaining_seconds = seconds - (hours + mins);
	QTime time(hours.count(), mins.count(), remaining_seconds.count());
	return time.toString().toStdString();
}

inline std::chrono::minutes string_to_minutes(const QString& min_string)
{
	QStringList parts = min_string.split(":");
	if (parts.size() != 2)
		return std::chrono::minutes{0};

	bool is_number;
	int hour = parts.at(0).toInt(&is_number);
	if (!is_number)
		return std::chrono::minutes{0};

	int minutes = parts.at(1).toInt(&is_number);
	if (!is_number)
		return std::chrono::minutes{0};

	return std::chrono::minutes{hour * 60 + minutes};
}

inline QDateTime get_today(const QDateTime& current_datetime, const QDateTime& previous_datetime = QDateTime())
{
	if (!previous_datetime.isValid())
		return current_datetime;

	if (current_datetime.date() == previous_datetime.date() && current_datetime >= previous_datetime)
		return previous_datetime;

	if (current_datetime.date() > previous_datetime.date())
		return current_datetime;

	// Otherwise current day is not valid
	return QDateTime();
}

inline double time_to_double(const QString& time)		// input: 3:30 -> 3.5
{
	QStringList parts = time.split(":");
	if (parts.size() != 2)
		return 0;

	int hour = parts.at(0).toInt();
	int minute = parts.at(1).toInt();

	if (minute >= 60)
		return 0;

	return hour + (minute / 60.0);
}

}

#endif // COMMONS_H
