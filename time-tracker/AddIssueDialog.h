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
	bool load_from_redmine() const;
	bool create_new_issue() const;

	int issue_id() const;

	QString get_subject() const;
	int get_state() const;

private:
	void load_categories();
	Ui::AddIssueDialog *ui;
};

#endif // ADDISSUEDIALOG_H
