#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <chrono>
#include <boost/format.hpp>

namespace commons
{

inline std::string to_string(const std::chrono::minutes& minutes)
{
	std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(minutes);
	std::chrono::minutes mins = minutes - hours;
	return (boost::format("%02u:%02u") % hours.count() % mins.count()).str();
}

inline std::string to_string(const std::chrono::seconds& seconds)
{
	std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);
	std::chrono::minutes mins = std::chrono::duration_cast<std::chrono::minutes>(seconds - hours);
	std::chrono::seconds remaining_seconds = seconds - (hours + mins);
	return (boost::format("%02u:%02u:%02u") % hours.count() % mins.count() % remaining_seconds.count()).str();
}

}

#endif // COMMONS_H
