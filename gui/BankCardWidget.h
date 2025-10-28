#pragma once
#include <QWidget>
#include <QTimer>
#include <optional>
#include <ctime>
#include "../core/feature/card/Card.h"

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
    Card* cardPtrFallback = nullptr;
    int id = -1;
    QString cardNumber;
    QString maskedNumber;
    QString currency;
    qint64 balanceMinor = 0;
    int dailyLimit = 0;
    double dailyUsageRatio = 1.0;
    QString status;
    std::optional<std::time_t> blockedUntil;

    QTimer countdownTimer;
    QPixmap designPixmap;
};
