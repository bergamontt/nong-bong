#pragma once

#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>
#include <vector>
#include "feature/scheduledTransfer/ScheduledTransfer.h"
#include "feature/currency/Currency.h"
#include "context/IContext.h"

class ScheduledTransferListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column {
        ColReceiver,
        ColAmount,
        ColFrequency,
        ColNext,
        ColDescription,
        ColComment,
        ColActive,
        ColCount
    };

    explicit ScheduledTransferListModel(const IContext& context, int fromCardId, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    ScheduledTransfer transferAt(int row) const;

    static constexpr int SortRole = Qt::UserRole + 1;

private:
    const IContext& _context;
    int _fromCardId = -1;
    std::vector<ScheduledTransfer> _scheduled;

    void updateScheduledTransfers();
    QString receiverText(const ScheduledTransfer& st) const;
};
