#pragma once
#include <string>
#include "Card.h"
#include "ICardDao.h"
#include "IBankTransactionDao.h"
#include "IExchangeRateDao.h"
#include "ICardService.h"

class CardService final : public ICardService
{
public:
    explicit CardService(ICardDao& cardDao, IBankTransactionDao& transDao, IExchangeRateDao& rateDao);
    CardService(const CardService&) = delete;
    CardService& operator=(const CardService&)& = delete;

private:
    ICardDao& _cardDao;
    IBankTransactionDao& _transDao;
    IExchangeRateDao& _rateDao;

    std::optional<Card> doGetCardById(int id) const override;
    std::optional<Card> doGetCardByNumber(const std::string& number) const override;

    std::vector<Card> doGetAllActiveCardsByUserId(int id) const override;
    std::vector<Card> doGetAllBlockedCardsByUserId(int id) const override;
    std::vector<Card> doGetAllDeletedCardsByUserId(int id) const override;
    std::vector<Card> doGetAllCardsByUserId(int id) const override;
    int doGetCardSpendingsSince(int cardId, const std::tm& time) const override;

    void doCreateCard(const Card& card) const override;
    void doUpdateCard(const Card& card) const override;
    void doDeleteCardById(int id) const override;

    bool doAccessToCard(int id, const std::string& pin) override;
    bool doChangeCardPin(int id, const std::string& oldPin, const std::string& newPin) const override;

    void handleFailedAccess(Card& card) const;
    void blockCardTemporarily(Card& card, int seconds) const;
};
