#pragma once
#include <QWidget>
#include <QTimer>
#include <optional>
#include <ctime>
#include "feature/card/Card.h"

class BankCardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BankCardWidget(QWidget* parent = nullptr);

    void setCard(const Card& card);
    void setDailyUsageRatio(double ratio);
    void setDesignPixmap(const QPixmap& pix);

protected:
    void paintEvent(QPaintEvent* event) override;
    QSize minimumSizeHint() const override;

private slots:
    void onCountdownTick();

private:
    qint64 blockedUntilEpochSeconds() const;

private:
    Card* _cardPtrFallback = nullptr;
    int _id = -1;
    QString _cardNumber;
    QString _currency;
    qint64 _balanceMinor = 0;
    int _dailyLimit = 0;
    double _dailyUsageRatio = 1.0;
    QString _status;
    std::optional<std::time_t> _blockedUntil;

    QTimer _countdownTimer;
    QPixmap _designPixmap;
};
