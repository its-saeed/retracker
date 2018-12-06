#include "IssueCategoryManager.h"
#include "ui_IssueCategoryManager.h"

#include "IssueTableWidget.h"
#include "UserPassDialog.h"
#include "DatabaseManager.h"

#include <QMessageBox>
#include <QMessageBox>

IssueCategoryManager::IssueCategoryManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IssueCategoryManager)
  , issue_manager(nullptr)
{
	ui->setupUi(this);
}

IssueCategoryManager::~IssueCategoryManager()
{
	delete ui;
}

void IssueCategoryManager::set_issue_manager(IssueManager* issue_man)
{
	issue_manager = issue_man;
	connect(issue_manager, &IssueManager::issue_updated, this, &IssueCategoryManager::on_issue_manager_issue_updated);
}

bool IssueCategoryManager::filter_issues(const QString& filter)
{
	if (filter.isEmpty())
		return false;

	for (auto & i : category_to_table_map)
		if (find_issue_in(i.first, i.second, filter))
			return true;

	return false;
}

void IssueCategoryManager::add_new_category_to_tab_widget(int catid, const QString& caption)
{
	// Create Table
	IssueTableWidget* tblw_new_issues = new IssueTableWidget;
	if (tblw_new_issues->columnCount() < 5)
		tblw_new_issues->setColumnCount(5);
	QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem(tr("شماره"));
	tblw_new_issues->setHorizontalHeaderItem(0, __qtablewidgetitem);
	QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem(tr("موضوع"));
	tblw_new_issues->setHorizontalHeaderItem(1, __qtablewidgetitem1);
	QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem(tr("زمان کل"));
	tblw_new_issues->setHorizontalHeaderItem(2, __qtablewidgetitem2);
	QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem(tr("زمان زده شده در پیگیر"));
	tblw_new_issues->setHorizontalHeaderItem(3, __qtablewidgetitem3);
	QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem(tr("زمان زده نشده در پیگیر"));
	tblw_new_issues->setHorizontalHeaderItem(4, __qtablewidgetitem4);
	tblw_new_issues->setObjectName(QStringLiteral("tblw_new_issues"));

	ui->tabw_issue_tables->insertTab(catid, tblw_new_issues, caption);
	ui->tabw_issue_tables->setTabIcon(catid, QIcon(":/ui/res/cat.png"));
	category_to_table_map.insert(std::make_pair(catid, tblw_new_issues));
	connect(tblw_new_issues, &IssueTableWidget::issue_selected, this, &IssueCategoryManager::issue_selected);
	connect(tblw_new_issues, &IssueTableWidget::issue_double_clicked, this, &IssueCategoryManager::issue_edit_requested);
}

void IssueCategoryManager::add_new_category(int catid, const QString& caption)
{
	if (!DatabaseManager::instance().add_category(catid, caption))
	{
		QMessageBox::warning(this, "New Category", "Can't add new category");
		return;
	}

	add_new_category_to_tab_widget(catid, caption);
}

void IssueCategoryManager::load_categories()
{
	auto cats = DatabaseManager::instance().all_categories();

	for (auto& cat : cats)
		add_new_category_to_tab_widget(cat.first, cat.second);
}

bool IssueCategoryManager::find_issue_in(int table_index, IssueTableWidget* table, const QString& filter)
{
	auto items = table->findItems(filter, Qt::MatchContains);
	if (items.size() > 0)
	{
		ui->tabw_issue_tables->setCurrentIndex(table_index);
		table->selectRow(items.at(0)->row());
		return true;
	}

	return false;
}

void IssueCategoryManager::on_led_filter_returnPressed()
{
	ui->led_filter->selectAll();
	if (filter_issues(this->ui->led_filter->text()))
		return;

	// Trying to add it to peygir if user wants
	if (!ui->chb_add_if_not_exist->isChecked())
		return;

	bool is_num;
	int issue_id = ui->led_filter->text().toInt(&is_num);
	if (!issue_id)
		return;

	Credential credit = UserPassDialog::get_credential();

	if (issue_manager->load_issue_from_peygir(issue_id, credit.username, credit.password))
	{
		update_issue_tables();		// TODO: change with more efficient method
		filter_issues(QString::number(issue_id));
	}
}

void IssueCategoryManager::on_issue_manager_issue_updated(Issue::Id id)
{
	const Issue& issue = issue_manager->get_issue_by_id(id);
	if (category_to_table_map.find(issue.get_state()) == category_to_table_map.end())
		// TODO: Log
		return;

	category_to_table_map[issue.get_state()]->update_issue(issue);
}

void IssueCategoryManager::update_issue_tables()
{
	for (auto kv : issue_manager->get_issues())
	{
		const Issue& issue = kv.second;
		if (category_to_table_map.find(issue.get_state()) == category_to_table_map.end())
			// TODO: Log
			continue;

		category_to_table_map[issue.get_state()]->add_issue(issue);
	}
}

void IssueCategoryManager::focus()
{
	ui->led_filter->setFocus();
	ui->led_filter->selectAll();
}

void IssueCategoryManager::on_btn_new_category_clicked()
{
	if (ui->led_new_category->text().isEmpty())
		return;

	add_new_category(ui->tabw_issue_tables->count(), ui->led_new_category->text());
}

void IssueCategoryManager::on_tabw_issue_tables_tabCloseRequested(int index)
{
	if (index <= 3)
	{
		QMessageBox::warning(this, "Category close", "Primary categories couldn't be closed.");
		return;
	}

	if (!DatabaseManager::instance().remove_category(index))
	{
		QMessageBox::warning(this, "Category close", "Removing failed");
		return;
	}

	delete ui->tabw_issue_tables->widget(index);
	ui->tabw_issue_tables->removeTab(index);
}
