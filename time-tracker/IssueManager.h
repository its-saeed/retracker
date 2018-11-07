#ifndef ISSUEMANAGER_H
#define ISSUEMANAGER_H

#include <QString>

class Issue
{
public:
private:
	quint32 id;
	QString subject;
	quint8 state;

};

class IssueManager
{
public:
	IssueManager();
};

#endif // ISSUEMANAGER_H
