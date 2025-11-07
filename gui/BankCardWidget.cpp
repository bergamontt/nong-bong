#include "BankCardWidget.h"

#include <iostream>
#include <QPainter>
#include <QLinearGradient>
#include <QFontMetrics>
#include <QDateTime>
#include <QDebug>
#include <QPainterPath>

using namespace std;

QString maskCardNumber(const QString& number) {
    QString digits = number;
    QString last4 = digits.right(4);
    return QString("**** **** **** %1").arg(last4);
}

QString formatMoney(qint64 minorUnits, const QString& currency) {
    return QString("%1 %2").arg(QString::number(minorUnits / 100.0, 'f', 2)).arg(currency);
}

QString humanFriendlyCountdown(qint64 seconds)
{
    if (seconds <= 0) 
        return QStringLiteral("00:00:00");
    qint64 h = seconds / 3600;
    qint64 m = (seconds % 3600) / 60;
    qint64 s = seconds % 60;
    return QString("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

BankCardWidget::BankCardWidget(QWidget* parent)
    : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumSize(260, 160);
    _countdownTimer.setInterval(1000);
    connect(&_countdownTimer, &QTimer::timeout, this, &BankCardWidget::onCountdownTick);
    setCursor(Qt::PointingHandCursor);
}

QSize BankCardWidget::minimumSizeHint() const
{
    return QSize(260, 160);
}

void BankCardWidget::setCard(const Card& card)
{
    _id = card.id;
    _cardNumber = QString::fromStdString(card.cardNumber);
    _currency = QString::fromStdString(card.currencyCode);
    _balanceMinor = static_cast<qint64>(card.balance);
    _dailyLimit = card.dailyLimit;
    _status = QString::fromStdString(statusToString(card.status));

    _blockedUntil.reset();
    if (card.blockedUntil.has_value()) {
        tm tm = *card.blockedUntil;
        time_t t = mktime(&tm);
        if (t != -1) 
            _blockedUntil = t;
    }

    if (_status == "blocked" && _blockedUntil.has_value()) {
        qint64 now = QDateTime::currentSecsSinceEpoch();
        if (static_cast<qint64>(*_blockedUntil) > now) {
            if (!_countdownTimer.isActive())
                _countdownTimer.start();
        }
        else {
            if (_countdownTimer.isActive())
                _countdownTimer.stop();
            _status = "active";
            _blockedUntil.reset();
        }
    }
    else {
        if (_countdownTimer.isActive())
            _countdownTimer.stop();
    }
    update();
}

int BankCardWidget::getCardId() const {
    return _id;
};

void BankCardWidget::setDailyUsageRatio(double ratio)
{
    if (ratio < 0.0) 
        ratio = 0.0;
    if (ratio > 1.0) 
        ratio = 1.0;
    _dailyUsageRatio = ratio;
    update();
}

void BankCardWidget::setDesignPixmap(const QPixmap& pix)
{
    _designPixmap = pix;
    update();
}

void BankCardWidget::setDesignPixmap()
{
    _designPixmap = QPixmap();
    update();
}

qint64 BankCardWidget::blockedUntilEpochSeconds() const
{
    if (!_blockedUntil.has_value())
        return 0;
    return static_cast<qint64>(*_blockedUntil);
}

void BankCardWidget::onCountdownTick()
{
    if (!_blockedUntil.has_value()) {
        _countdownTimer.stop();
        update();
        return;
    }
    qint64 now = QDateTime::currentSecsSinceEpoch();
    qint64 until = static_cast<qint64>(*_blockedUntil) - now;
    if (until <= 0) {
        _blockedUntil.reset();
        _status = "active";
        _countdownTimer.stop();
    }
    update();
}

void BankCardWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    //QWidget::mousePressEvent(event);
}


void BankCardWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF r = rect().adjusted(6, 6, -6, -6);
    const qreal radius = qMin(r.height(), r.width()) * 0.06;

    if (_designPixmap.isNull()) {
        QLinearGradient g(r.topLeft(), r.bottomRight());
        g.setColorAt(0.0, QColor(28, 118, 211));
        g.setColorAt(1.0, QColor(19, 84, 150));
        p.setBrush(g);
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(r, radius, radius);
    }
    else {
        QPixmap scaled = _designPixmap.scaled(r.size().toSize(),
            Qt::KeepAspectRatioByExpanding,
            Qt::SmoothTransformation);
        QPainterPath clipPath;
        clipPath.addRoundedRect(r, radius, radius);
        p.save();
        p.setOpacity(0.6);
        p.setClipPath(clipPath);
        const qreal dx = (scaled.width() - r.width()) / 2.0;
        const qreal dy = (scaled.height() - r.height()) / 2.0;
        QPointF drawPos = r.topLeft() - QPointF(dx, dy);
        p.drawPixmap(drawPos, scaled);
        p.restore();

        p.setPen(QPen(QColor(0, 0, 0, 60), 1));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(r, radius, radius);
    }

    QRectF chipRect(r.left() + r.width() * 0.06, r.top() + r.height() * 0.14,
        r.width() * 0.12, r.height() * 0.16);
    p.setBrush(QColor(255, 215, 140));
    p.setPen(QColor(200, 160, 90));
    p.drawRoundedRect(chipRect, 4, 4);
    p.setPen(QColor(180, 140, 80));
    for (int i = 1; i <= 2; ++i) {
        qreal x = chipRect.left() + i * chipRect.width() / 3.0;
        p.drawLine(QPointF(x, chipRect.top() + 4), QPointF(x, chipRect.bottom() - 4));
    }
    p.setPen(Qt::NoPen);

    qreal h = r.height();
    QFont numFont = font();
    numFont.setBold(true);
    numFont.setPointSizeF(h * 0.11);
    p.setFont(numFont);
    p.setPen(Qt::white);

    QString displayNumber = maskCardNumber(_cardNumber);
    QRectF numArea(r.left() + r.width() * 0.06, r.top() + r.height() * 0.38,
        r.width() * 0.88, r.height() * 0.2);
    p.drawText(numArea, Qt::AlignLeft | Qt::AlignVCenter, displayNumber);

    QFont balFont = font();
    balFont.setBold(true);
    balFont.setPointSizeF(h * 0.085);
    p.setFont(balFont);
    QString balText = formatMoney(_balanceMinor, _currency);
    QRectF balArea(
        r.left() + r.width() * 0.52,
        r.top() + r.height() * 0.12,
        r.width() * 0.42,
        r.height() * 0.18
    );
    p.drawText(balArea, Qt::AlignRight | Qt::AlignTop, balText);

    QFont smallFont = font();
    smallFont.setPointSizeF(h * 0.065);
    p.setFont(smallFont);
    QString dailyLabel = QString("Daily limit: %1").arg(formatMoney(_dailyLimit, _currency));
    QRectF dailyLabelArea(r.left() + r.width() * 0.06, r.bottom() - r.height() * 0.22,
        r.width() * 0.6, r.height() * 0.08);
    p.drawText(dailyLabelArea, Qt::AlignLeft | Qt::AlignVCenter, dailyLabel);

    qreal barHeight = qMax(6.0, h * 0.06);
    QRectF barBg(r.left() + r.width() * 0.06, r.bottom() - barHeight - r.height() * 0.04,
        r.width() * 0.88, barHeight);

    p.setBrush(QColor(0, 0, 0, 80));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(barBg, barHeight / 2.0, barHeight / 2.0);

    QRectF fillRect = barBg;
    fillRect.setWidth(barBg.width() * _dailyUsageRatio);
    p.setBrush(QColor(0, 200, 100));
    p.drawRoundedRect(fillRect, barHeight / 2.0, barHeight / 2.0);

    p.setPen(Qt::white);
    QFont pctFont = font();
    pctFont.setPointSizeF(h * 0.055);
    pctFont.setBold(true);
    p.setFont(pctFont);
    QString pct = (_dailyUsageRatio >= 0.0)
        ? QString("%1%").arg(int(_dailyUsageRatio * 100.0))
        : QString::number(_dailyLimit);
    QRectF pctArea(barBg.right() - 80, barBg.top(), 80, barBg.height());
    p.drawText(pctArea, Qt::AlignRight | Qt::AlignVCenter, pct);

    if (_status == "blocked" && _blockedUntil.has_value()) {
        qint64 now = QDateTime::currentSecsSinceEpoch();
        qint64 until = static_cast<qint64>(*_blockedUntil) - now;
        if (until > 0) {
            p.setBrush(QColor(255, 255, 255, 180));
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(r, radius, radius);

            QString cd = humanFriendlyCountdown(until);
            QFont cdFont = font();
            cdFont.setBold(true);
            cdFont.setPointSizeF(h * 0.14);
            p.setFont(cdFont);
            p.setPen(QColor(180, 30, 30));
            p.drawText(r, Qt::AlignCenter, QString("Blocked\n%1").arg(cd));
            return;
        }
    }
}
