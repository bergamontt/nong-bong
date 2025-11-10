#include "ScheduledTransferListModel.h"
#include "feature/currency/ICurrencyService.h"
#include "feature/scheduledTransfer/IScheduledTransferService.h"
#include "feature/card/CardService.h"
#include <QString>
#include <cmath>

using namespace std;

qint64 tmToEpoch(const tm& t);
QString groupedCardNumber(const string& number);

QString formatAmount(int amountMinor, const Currency& currency)
{
    int minor = currency.minorUnit;
    int fracDigits = 0;
    int mu = minor;
    while (mu > 1 && fracDigits < 9)
    {
        mu /= 10;
        fracDigits++;
    }
    qint64 whole = amountMinor / minor;
    int frac = abs(amountMinor % minor);
    QString fracStr;
    if (fracDigits > 0)
        fracStr = QString(".%1").arg(frac, fracDigits, 10, QChar('0'));
    return QString("%1%2 %3").arg(whole).arg(fracStr)
        .arg(QString::fromStdString(currency.code));
}

ScheduledTransferListModel::ScheduledTransferListModel(const IContext& context, int fromCardId, QObject* parent)
    : QAbstractTableModel(parent), _context(context), _fromCardId(fromCardId)
{
    updateScheduledTransfers();
}

int ScheduledTransferListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return int(_scheduled.size());
}

int ScheduledTransferListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColCount;
}

QVariant ScheduledTransferListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const ScheduledTransfer& st = _scheduled.at(index.row());
    int col = index.column();

    if (role == SortRole)
    {
        if (col == ColNext && st.active)
            return QVariant::fromValue<qint64>(tmToEpoch(st.nextTun.value()));
        else
            return QVariant::fromValue<qint64>(0);
    }

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case ColReceiver:
            return receiverText(st);
        case ColAmount:
        {
            const ICurrencyService& service = _context.currencyService();
            Currency c = service.getCurrencyByCode(st.currencyCode).value();
            return formatAmount(st.amount, c);
        }
        case ColFrequency:
            return QString::fromStdString(st.frequency);
        case ColNext:
        {
            if (st.active)
            {
                qint64 epoch = tmToEpoch(st.nextTun.value());
                QDateTime dt = QDateTime::fromSecsSinceEpoch(epoch);
                return dt.toString(QLocale().dateFormat(QLocale::ShortFormat));
            }
            else
                return QString("-");
        }
        case ColDescription:
            return QString::fromStdString(st.description);
        case ColComment:
            return st.comment ? QString::fromStdString(*st.comment) : QString();
        case ColActive:
            return st.active ? true : false;
        }
    }

    if (role == Qt::ToolTipRole) {
        if (index.column() == ColReceiver)
            return receiverText(st);
        if (index.column() == ColDescription)
            return QString::fromStdString(st.description);
        if (index.column() == ColComment && st.comment.has_value())
            return QString::fromStdString(*st.comment);
    }

    if (role == Qt::TextAlignmentRole)
    {
        if (col == ColAmount)
            return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return {};
}

QVariant ScheduledTransferListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return {};
    switch (section) {
    case ColReceiver:
        return QStringLiteral("Receiver");
    case ColAmount:
        return QStringLiteral("Amount");
    case ColFrequency:
        return QStringLiteral("Frequency");
    case ColNext:
        return QStringLiteral("Next transfer");
    case ColDescription:
        return QStringLiteral("Description");
    case ColComment:
        return QStringLiteral("Comment");
    case ColActive:
        return QStringLiteral("Active");
    default:
        return {};
    }
}

Qt::ItemFlags ScheduledTransferListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

ScheduledTransfer ScheduledTransferListModel::transferAt(int row) const
{
    return _scheduled.at(row);
}

void ScheduledTransferListModel::updateScheduledTransfers()
{
    const IScheduledTransferService& service = _context.scheduledTransferService();
    _scheduled = service.getAllScheduledTransfersFromCardId(_fromCardId);
    std::sort(_scheduled.begin(), _scheduled.end(),
        [this](const ScheduledTransfer& a, const ScheduledTransfer& b) {
            if (!a.active && !b.active)
                return true;
            if (a.active && !b.active)
                return true;
            if (!a.active && b.active)
                return false;
            return tmToEpoch(a.nextTun.value()) > tmToEpoch(b.nextTun.value());
        });
}

QString ScheduledTransferListModel::receiverText(const ScheduledTransfer& st) const
{
    Card card = _context.cardService().getCardById(st.toCardId).value();
    return groupedCardNumber(card.cardNumber);
}
