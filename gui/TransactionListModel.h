#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>
#include <vector>
#include <map>
#include "feature/transaction/BankTransaction.h"
#include "feature/currency/Currency.h"
#include "context/IContext.h"

class TransactionListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column {
        ColDate = 0,
        ColType,
        ColCounterparty,
        ColAmount,
        ColDescription,
        ColComment,
        ColStatus,
        ColCount
    };

    explicit TransactionListModel(const IContext& context, int cardId, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    BankTransaction transactionAt(int row) const;

    static constexpr int SortRole = Qt::UserRole + 1;

private:
    const IContext& _context;
    int _cardId = -1;
    std::vector<BankTransaction> _transactions;

    void updateTransactions();
    QString formatAmount(int amountMinor, const Currency& currency, bool outgoing) const;
    QString counterpartyText(const BankTransaction& tx) const;
};
