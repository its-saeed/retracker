#ifndef APPLYTIMETOPAYGIRDIALOG_H
#define APPLYTIMETOPAYGIRDIALOG_H

#include <QDialog>

namespace Ui {
class ApplyTimeToPaygirDialog;
}

class ApplyTimeToPaygirDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ApplyTimeToPaygirDialog(QWidget *parent = 0);
	~ApplyTimeToPaygirDialog();

private:
	Ui::ApplyTimeToPaygirDialog *ui;
};

#endif // APPLYTIMETOPAYGIRDIALOG_H
