#include "IssueCategoryManager.h"
#include "ui_IssueCategoryManager.h"

IssueCategoryManager::IssueCategoryManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IssueCategoryManager)
{
	ui->setupUi(this);
}

IssueCategoryManager::~IssueCategoryManager()
{
	delete ui;
}
