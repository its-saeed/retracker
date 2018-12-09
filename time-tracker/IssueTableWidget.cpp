#include "IssueTableWidget.h"
#include <QHeaderView>
#include <QMenu>
#include <QCursor>
#include <QDesktopServices>
#include <QUrl>

IssueTableWidget::IssueTableWidget(QWidget* parent)
: QTableWidget(parent)
{
	setSelectionBehavior(QAbstractItemView::SelectRows);
	connect(this, &IssueTableWidget::cellClicked, this, &IssueTableWidget::item_clicked);
	connect(this, &IssueTableWidget::cellDoubleClicked, this, &IssueTableWidget::on_item_double_clicked);
	auto header = horizontalHeader();
	header->setSectionResizeMode(QHeaderView::ResizeToContents);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setAlternatingRowColors(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &IssueTableWidget::customContextMenuRequested, this, &IssueTableWidget::on_context_menu_requested);
}

void IssueTableWidget::add_issue(const Issue& issue)
{
	if (issue_id_to_table_row_map.contains(issue.get_id()))
		return;

	int row_count = rowCount();
	setRowCount(row_count + 1);

	// Issue ID Item in table
	auto issue_id_item = new QTableWidgetItem(QString::number(issue.get_id()));
	setItem(row_count, ISSUE_ID_COLUMN, issue_id_item);

	auto issue_subject_item = new QTableWidgetItem(issue.get_subject());
	setItem(row_count, ISSUE_SUBJECT_COLUMN, issue_subject_item);

	auto issue_total_time_item = new QTableWidgetItem(issue.get_total_duration_string());
	setItem(row_count, ISSUE_TOTAL_TIME_COLUMN, issue_total_time_item);

	auto issue_total_applied_time_item = new QTableWidgetItem(issue.get_total_duration_string(true));
	setItem(row_count, ISSUE_TOTAL_APPLIED_TIME_COLUMN, issue_total_applied_time_item);

	auto issue_total_unapplied_time_item = new QTableWidgetItem(issue.get_total_duration_string(false));
	setItem(row_count, ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN, issue_total_unapplied_time_item);

	issue_id_to_table_row_map.insert(issue.get_id(), row_count);
}

void IssueTableWidget::update_issue(const Issue& issue)
{
	if (!issue_id_to_table_row_map.contains(issue.get_id()))
		return;

	int row_count = issue_id_to_table_row_map.value(issue.get_id());
	item(row_count, ISSUE_SUBJECT_COLUMN)->setText(issue.get_subject());
	item(row_count, ISSUE_TOTAL_TIME_COLUMN)->setText(issue.get_total_duration_string());
	item(row_count, ISSUE_TOTAL_APPLIED_TIME_COLUMN)->setText(issue.get_total_duration_string(true));
	item(row_count, ISSUE_TOTAL_UNAPPLIED_TIME_COLUMN)->setText(issue.get_total_duration_string(false));
}

void IssueTableWidget::on_context_menu_requested(const QPoint& pos)
{
	if (itemAt(pos) == nullptr)
		return;

	QMenu menu;
	menu.addAction(QIcon(":/ui/res/upload.png"), "Open",
				   [this](){
		QDesktopServices::openUrl(QUrl("http://projects.mahsan.co/issues/" + QString::number(get_currently_selected_issue_id())));
	});
	menu.addSeparator();
	menu.addAction(QIcon(":/ui/res/delete2.png"), "Delete");
	menu.addAction("Move to");
	menu.addSeparator();
	menu.addAction(QIcon(":/ui/res/play2.png"), "Start", [this](){this->issue_selected(get_currently_selected_issue_id());});
	menu.addAction(QIcon(":/ui/res/stop3.png"), "Stop");
	menu.addSeparator();
	menu.addAction(QIcon(":/ui/res/timer.png"), "Timeslice Editor", [this](){
		this->issue_double_clicked(get_currently_selected_issue_id());
	});
	menu.exec(QCursor::pos());
}

void IssueTableWidget::item_clicked(int row, int)
{
	Issue::Id issue_id = item(row, IssueTableWidget::ISSUE_ID_COLUMN)->text().toInt();
	emit issue_selected(issue_id);
}

void IssueTableWidget::on_item_double_clicked(int row, int col)
{
	Issue::Id issue_id = item(row, IssueTableWidget::ISSUE_ID_COLUMN)->text().toInt();
	emit issue_double_clicked(issue_id);
}

Issue::Id IssueTableWidget::get_currently_selected_issue_id() const
{
	if (currentItem() == nullptr)
		return 0;

	return item(currentRow(), IssueTableWidget::ISSUE_ID_COLUMN)->text().toInt();
}
