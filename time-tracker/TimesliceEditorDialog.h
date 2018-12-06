#ifndef TIMESLICEEDITORDIALOG_H
#define TIMESLICEEDITORDIALOG_H

#include <QDialog>

namespace Ui {
class TimesliceEditorDialog;
}

class TimesliceEditorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit TimesliceEditorDialog(QWidget *parent = nullptr);
	~TimesliceEditorDialog();

private:
	Ui::TimesliceEditorDialog *ui;
};

#endif // TIMESLICEEDITORDIALOG_H
