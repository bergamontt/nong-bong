#pragma once
#include <QWidget>
#include <QTableView>
#include <memory>
#include "TransactionListModel.h"
#include "context/IContext.h"

class TransactionListWidget : public QWidget {
    Q_OBJECT
public:
    explicit TransactionListWidget(const IContext& context, int cardId, QWidget* parent = nullptr);

signals:
    void transactionActivated(int transactionId);

private:
    QTableView* _table;
    TransactionListModel* _model;
};
