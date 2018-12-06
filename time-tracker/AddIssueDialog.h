#ifndef ADDISSUEDIALOG_H
#define ADDISSUEDIALOG_H

#include <QDialog>

namespace Ui {
class AddIssueDialog;
}

class AddIssueDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddIssueDialog(QWidget *parent = nullptr);
	~AddIssueDialog();

private:
	Ui::AddIssueDialog *ui;
};

#endif // ADDISSUEDIALOG_H
