#include "TodaySummaryWidget.h"
#include "ui_TodaySummaryWidget.h"

TodaySummaryWidget::TodaySummaryWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::TodaySummaryWidget)
{
	ui->setupUi(this);
}

TodaySummaryWidget::~TodaySummaryWidget()
{
	delete ui;
}
