#ifndef PROCCESSRUNNER_H
#define PROCCESSRUNNER_H

#include <QProcess>

class ProccessRunner : public QProcess
{
public:
	ProccessRunner(const QString& command, const QStringList params);
	int start_and_wait();
};

#endif // PROCCESSRUNNER_H
