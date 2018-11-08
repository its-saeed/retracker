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
	setItem(row_count, ISSUE_ID_COLUMN, new QTableWidgetItem(QString::number(issue.get_id())));
	setItem(row_count, ISSUE_SUBJECT_COLUMN, new QTableWidgetItem(issue.get_subject()));
	setItem(row_count, ISSUE_TOTAL_TIME_COLUMN, new QTableWidgetItem(issue.total_duration_string()));
	setItem(row_count, ISSUE_TOTAL_APPLIED_TIME_COLUMN, new QTableWidgetItem(issue.total_applied_duration_string()));
	setItem(row_count, ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN, new QTableWidgetItem(issue.total_unapplied_duration_string()));
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

void IssueTableWidget::item_clicked(int row, int col)
{
	Issue::Id issue_id = item(row, IssueTableWidget::ISSUE_ID_COLUMN)->text().toInt();
	emit issue_selected(issue_id);
}
