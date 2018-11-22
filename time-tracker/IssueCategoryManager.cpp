#include "IssueCategoryManager.h"
#include "ui_IssueCategoryManager.h"

#include "UserPassDialog.h"

IssueCategoryManager::IssueCategoryManager(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::IssueCategoryManager)
  , issue_manager(nullptr)
{
	ui->setupUi(this);
	add_new_category(Issue::State::NEW, ui->tblw_new_issues);
	add_new_category(Issue::State::DOING, ui->tblw_doing_issues);
	add_new_category(Issue::State::RETURNED, ui->tblw_returned_issues);
	add_new_category(Issue::State::QUALITY_CHECK, ui->tblw_returned_issues);
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
	{
		if (find_issue_in(i.first, i.second, filter))
			return true;
	}

	return false;
}

void IssueCategoryManager::add_new_category(int catid, IssueTableWidget* table)
{
	category_to_table_map.insert(std::make_pair(catid, table));
	connect(table, &IssueTableWidget::issue_selected, this, &IssueCategoryManager::issue_selected);
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
