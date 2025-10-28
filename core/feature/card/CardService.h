#pragma once
#include "Card.h"
#include "ICardDao.h"
#include "ICardService.h"

class CardService final : public ICardService
{
public:
    explicit CardService(ICardDao& dao);
    CardService(const CardService&) = delete;
    CardService& operator=(const CardService&)& = delete;

private:
    ICardDao& _cardDao;

    std::optional<Card> doGetCardById(int id) const override;

    std::vector<Card> doGetAllActiveCardsByUserId(int id) const override;
    std::vector<Card> doGetAllBlockedCardsByUserId(int id) const override;
    std::vector<Card> doGetAllDeletedCardsByUserId(int id) const override;
    std::vector<Card> doGetAllCardsByUserId(int id) const override;

    void doCreateCard(const Card& card) const override;
    void doUpdateCard(const Card& card) const override;
    void doDeleteCardById(int id) const override;

    bool doAccessToCard(int id, const std::string& pin) const override;
    bool doChangeCardPin(int id, const std::string& oldPin, const std::string& newPin) const override;
};
