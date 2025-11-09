#include "TransactionListWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>

TransactionListWidget::TransactionListWidget(const IContext& context, int cardId, QWidget* parent)
    : QWidget(parent), _table(new QTableView(this)),
    _model(new TransactionListModel(context, cardId, this))
{
    _table->setModel(_model);
    _table->setSelectionBehavior(QAbstractItemView::SelectRows);
    _table->setSelectionMode(QAbstractItemView::SingleSelection);
    _table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _table->horizontalHeader()->setStretchLastSection(true);
    _table->setAlternatingRowColors(true);
    _table->verticalHeader()->setVisible(false);

    _table->setSortingEnabled(false);

    connect(_table, &QTableView::doubleClicked, this, [this](const QModelIndex& index) 
        {
        if (!index.isValid()) 
            return;
        int row = index.row();
        BankTransaction tx = _model->transactionAt(row);
        emit transactionActivated(tx.id);
        });

    auto* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->addWidget(_table);
    setLayout(lay);
}