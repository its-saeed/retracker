#include "Settings.h"

#include <QDateTime>

Settings::Settings()
: QSettings(QString(ORGANIATION_NAME), QString(APPLICATION_NAME))
{
}

void Settings::save_username(const QString& user)
{
	setValue(QString(USER_USERNAME_KEY), user);
}

QString Settings::get_username() const
{
	return value(USER_USERNAME_KEY, "").toString();
}

void Settings::save_password(const QString& pass)
{
	setValue(QString(USER_PASSWORD_KEY), pass);
}

QString Settings::get_password() const
{
	return value(USER_PASSWORD_KEY, "").toString();
}

void Settings::set_remember_userpass(bool remember)
{
	setValue(QString(USER_PASSWORD_REMEMBER), remember);
}

bool Settings::remember_userpass() const
{
	return value(USER_PASSWORD_REMEMBER, false).toBool();
}

void Settings::save_today(const QDateTime& today)
{
	setValue(TODAY_KEY, today);
}

QDateTime Settings::get_today() const
{
	return value(TODAY_KEY, QDateTime()).toDateTime();
}

Settings& Settings::instance()
{
	static Settings settings;
	return settings;
}
