#include "TransactionListModel.h"
#include "feature/currency/ICurrencyService.h"
#include "feature/transaction/IBankTransactionService.h"
#include "feature/card/CardService.h"
#include <QString>
#include <cmath>

using namespace std;

qint64 tmToEpoch(const tm& t) 
{
    tm copy = t;
    time_t tt = mktime(&copy);
    return static_cast<qint64>(tt);
}

QString groupedCardNumber(const string& number) {
    QString s = QString::fromStdString(number);
    QStringList parts;
    for (int i = 0; i < s.length(); i += 4)
        parts << s.mid(i, 4);
    return parts.join(' ');
}

TransactionListModel::TransactionListModel(const IContext& context, int cardId, QObject* parent)
    : QAbstractTableModel(parent), _context(context), _cardId(cardId)
{
        updateTransactions();
}

int TransactionListModel::rowCount(const QModelIndex& parent) const 
{
    Q_UNUSED(parent);
    return int(_transactions.size());
}

int TransactionListModel::columnCount(const QModelIndex& parent) const 
{
    Q_UNUSED(parent);
    return ColCount;
}

QVariant TransactionListModel::data(const QModelIndex& index, int role) const 
{
    if (!index.isValid()) 
        return {};

    const BankTransaction& tx = _transactions.at(index.row());
    int col = index.column();

    if (role == SortRole) 
    {
        if (col == ColDate) 
            return QVariant::fromValue<qint64>(tmToEpoch(tx.createdAt));
    }

    if (role == Qt::DisplayRole) 
    {
        switch (col) 
        {
        case ColDate: 
        {
            qint64 epoch = tmToEpoch(tx.createdAt);
            QDateTime dt = QDateTime::fromSecsSinceEpoch(epoch);
            return dt.toString(QLocale().dateFormat(QLocale::ShortFormat));
        }
        case ColType:
            return QString::fromStdString(tx.type);
        case ColCounterparty:
            return counterpartyText(tx);
        case ColAmount: 
        {
            const ICurrencyService& service = _context.currencyService();
            Currency c = service.getCurrencyByCode(tx.currencyCode).value();
            bool outgoing = (tx.fromCardId && _cardId == *tx.fromCardId);
            return formatAmount(tx.amount, c, outgoing);
        }
        case ColDescription:
            return QString::fromStdString(tx.description);
        case ColComment:
            return tx.comment ? QString::fromStdString(*tx.comment) : QString();
        case ColStatus:
            return QString::fromStdString(tx.status);
        }
    }

    if (role == Qt::ToolTipRole) {
        if (index.column() == ColCounterparty)
            return counterpartyText(tx);
        if (index.column() == ColDescription)
            return QString::fromStdString(tx.description);
        if (index.column() == ColComment && tx.comment.has_value())
            return QString::fromStdString(*tx.comment);
    }

    if (role == Qt::TextAlignmentRole) 
    {
        if (col == ColAmount) 
            return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        return QVariant::fromValue(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return {};
}

QVariant TransactionListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) 
        return {};
    switch (section) {
    case ColDate: 
        return QStringLiteral("Date");
    case ColType: 
        return QStringLiteral("Type");
    case ColCounterparty: 
        return QStringLiteral("Counterparty");
    case ColAmount: 
        return QStringLiteral("Amount");
    case ColDescription: 
        return QStringLiteral("Description");
    case ColComment: 
        return QStringLiteral("Comment");
    case ColStatus: 
        return QStringLiteral("Status");
    default: 
        return {};
    }
}

Qt::ItemFlags TransactionListModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) 
        return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

BankTransaction TransactionListModel::transactionAt(int row) const 
{
    return _transactions.at(row);
}

void TransactionListModel::updateTransactions() 
{
    const IBankTransactionService& service = _context.bankTransactionService();
    _transactions = service.getBankTransactionByUserCardId(_cardId);
    std::sort(_transactions.begin(), _transactions.end(),
        [this](const BankTransaction& a, const BankTransaction& b) {
            return tmToEpoch(a.createdAt) > tmToEpoch(b.createdAt);
        });
}

QString TransactionListModel::formatAmount(int amountMinor, const Currency& currency, bool outgoing) const 
{
    QString sign = outgoing ? "-" : "";
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
    return QString("%1%2%3 %4").arg(sign).arg(whole).arg(fracStr)
        .arg(QString::fromStdString(currency.code));
}

QString TransactionListModel::counterpartyText(const BankTransaction& tx) const {
    if (tx.fromCardId && _cardId == *tx.fromCardId) 
    {
        if (tx.toCardId) 
        {
            Card card = _context.cardService().getCardById(*tx.toCardId).value();
            return QString("To: %1").arg(groupedCardNumber(card.cardNumber));
        }
        return QString("To: ATM");
    }
    if (tx.toCardId && _cardId == *tx.toCardId) 
    {
        if (tx.fromCardId) 
        {
            Card card = _context.cardService().getCardById(*tx.fromCardId).value();
            return QString("From: %1").arg(groupedCardNumber(card.cardNumber));
        }
        return QString("From: ATM");
    }
    return QString("-");
}
