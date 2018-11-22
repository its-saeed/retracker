#ifndef ISSUECATEGORYMANAGER_H
#define ISSUECATEGORYMANAGER_H

#include <QWidget>

namespace Ui {
class IssueCategoryManager;
}

class IssueCategoryManager : public QWidget
{
	Q_OBJECT

public:
	explicit IssueCategoryManager(QWidget *parent = nullptr);
	~IssueCategoryManager();

private:
	Ui::IssueCategoryManager *ui;
};

#endif // ISSUECATEGORYMANAGER_H
