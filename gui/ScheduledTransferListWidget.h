#pragma once
#include <QWidget>
#include <QTableView>
#include <memory>
#include "ScheduledTransferListModel.h"
#include "context/IContext.h"

class ScheduledTransferListWidget : public QWidget {
    Q_OBJECT
public:
    explicit ScheduledTransferListWidget(const IContext& context, int fromCardId, QWidget* parent = nullptr);

signals:
    void transferActivated(int transferId);

private:
    QTableView* _table;
    ScheduledTransferListModel* _model;
};
