#pragma once
#include <QPushButton>
#include <QLabel>
#include <QPropertyAnimation>
#include <QWidget>
#include <QTimer>
#include <optional>
#include <ctime>
#include <optional>
#include "feature/card/Card.h"
#include "context/IContext.h"
#include "BankCardWidget.h"

class BankCardList : public QWidget
{
    Q_OBJECT
        Q_PROPERTY(int carouselShift READ carouselShift WRITE setCarouselShift)

public:
    explicit BankCardList(IContext& context, int userId, 
        QPushButton* prevBtn, QPushButton* nextBtn, QWidget* parent = nullptr);

    std::optional<Card> getSelectedCard();

signals:
    void selectedCardChanged(const Card& card);

public slots:
    void refresh();

protected:
    void paintEvent(QPaintEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onPrevClicked();
    void onNextClicked();
    void onAnimationFinished();

private:
    IContext& _context;
    int _userId;
    std::vector<Card> _cards;
    int _selectedIndex = -1;
    int _pendingIndex = -1;

    BankCardWidget* _leftCard = nullptr;
    BankCardWidget* _centerCard = nullptr;
    BankCardWidget* _rightCard = nullptr;

    QPushButton* _prevBtn = nullptr;
    QPushButton* _nextBtn = nullptr;
    QLabel* _emptyLabel = nullptr;

    QPropertyAnimation* _anim = nullptr;
    int _carouselShift = 0;

    void updateCards();
    void updateVisibleWidgets();
    void layoutWidgets();

    int carouselShift() const;
    void setCarouselShift(int s);
};