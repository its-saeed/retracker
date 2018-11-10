#include "IssueTableWidget.h"
#include <QHeaderView>
#include <QDebug>

IssueTableWidget::IssueTableWidget(QWidget* parent)
: QTableWidget(parent)
{
	setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(this, &IssueTableWidget::cellDoubleClicked, this, &IssueTableWidget::item_clicked);
	auto header = horizontalHeader();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void IssueTableWidget::add_issue(const Issue& issue)
{
	int row_count = rowCount();
	setRowCount(row_count + 1);

	// Issue ID Item in table
	auto issue_id_item = new QTableWidgetItem(QString::number(issue.get_id()));
	setItem(row_count, ISSUE_ID_COLUMN, issue_id_item);
	set_color(issue_id_item, row_count);

	auto issue_subject_item = new QTableWidgetItem(issue.get_subject());
	setItem(row_count, ISSUE_SUBJECT_COLUMN, issue_subject_item);
	set_color(issue_subject_item, row_count);

	auto issue_total_time_item = new QTableWidgetItem(issue.total_duration_string());
	setItem(row_count, ISSUE_TOTAL_TIME_COLUMN, issue_total_time_item);
	set_color(issue_total_time_item, row_count);

	auto issue_total_applied_time_item = new QTableWidgetItem(issue.total_applied_duration_string());
	setItem(row_count, ISSUE_TOTAL_APPLIED_TIME_COLUMN, issue_total_applied_time_item);
	set_color(issue_total_applied_time_item, row_count);

	auto issue_total_unapplied_time_item = new QTableWidgetItem(issue.total_unapplied_duration_string());
	setItem(row_count, ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN, issue_total_unapplied_time_item);
	set_color(issue_total_unapplied_time_item, row_count);

	issue_id_to_table_row_map.insert(issue.get_id(), row_count);
}

void IssueTableWidget::update_issue(const Issue& issue)
{
	if (!issue_id_to_table_row_map.contains(issue.get_id()))
		return;

	int row_count = issue_id_to_table_row_map.value(issue.get_id());
	item(row_count, ISSUE_SUBJECT_COLUMN)->setText(issue.get_subject());
	item(row_count, ISSUE_TOTAL_TIME_COLUMN)->setText(issue.total_duration_string());
	item(row_count, ISSUE_TOTAL_APPLIED_TIME_COLUMN)->setText(issue.total_applied_duration_string());
	item(row_count, ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN)->setText(issue.total_unapplied_duration_string());
}

void IssueTableWidget::item_clicked(int row, int)
{
	Issue::Id issue_id = item(row, IssueTableWidget::ISSUE_ID_COLUMN)->text().toInt();
	emit issue_selected(issue_id);
}

void IssueTableWidget::set_color(QTableWidgetItem* item, int row)
{
	if (row % 2)
		item->setBackground(QBrush(QColor("lightgray")));
}
