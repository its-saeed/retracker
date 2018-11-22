#ifndef TODAYSUMMARYWIDGET_H
#define TODAYSUMMARYWIDGET_H

#include <QWidget>

namespace Ui {
class TodaySummaryWidget;
}

class TodaySummaryWidget : public QWidget
{
	Q_OBJECT

public:
	explicit TodaySummaryWidget(QWidget *parent = nullptr);
	~TodaySummaryWidget();

private:
	Ui::TodaySummaryWidget *ui;
};

#endif // TODAYSUMMARYWIDGET_H
