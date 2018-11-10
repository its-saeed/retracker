#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
	static Settings& instance();

	void save_username(const QString& user);
	QString get_username() const;

	void save_password(const QString& pass);
	QString get_password() const;

	void set_remember_userpass(bool remember);
	bool remember_userpass() const;

	void save_today(const QDateTime& today);
	QDateTime get_today() const;

private:
	Settings();
	static constexpr const char* ORGANIATION_NAME = "Mahsan";
	static constexpr const char* APPLICATION_NAME = "Tracker";

	// User, Pass key
	static constexpr const char* USER_USERNAME_KEY = "user/name";
	static constexpr const char* USER_PASSWORD_KEY = "user/pass";
	static constexpr const char* USER_PASSWORD_REMEMBER = "user/remember_userpass";

	// Date and time keys
	static constexpr const char* TODAY_KEY = "datetime/today";

};

#endif // SETTINGS_H
