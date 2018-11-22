#include "ProccessRunner.h"

ProccessRunner::ProccessRunner(const QString& command, const QStringList params)
{
	setProgram(command);
	setArguments(params);
}

int ProccessRunner::start_and_wait()
{
	start();
	waitForFinished();
	return exitCode();
}
