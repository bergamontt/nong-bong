#pragma once
#include "Card.h"
#include "ICardDao.h"
#include "ICardService.h"

class CardService final : public ICardService{

    std::shared_ptr<ICardDao> dao;

    explicit CardService(std::shared_ptr<ICardDao> dao);

    [[nodiscard]] std::optional<Card> doGetCardById(int id) const override;

    [[nodiscard]] std::vector<Card> doGetAllActiveCardsByUserId(int id) const override;
    [[nodiscard]] std::vector<Card> doGetAllBlockedCardsByUserId(int id) const override;
    [[nodiscard]] std::vector<Card> doGetAllDeletedCardsByUserId(int id) const override;
    [[nodiscard]] std::vector<Card> doGetAllCardsByUserId(int id) const override;

    void doCreateCard(const Card& card) const override;
    void doUpdateCard(const Card& card) const override;
    void doDeleteCardById(int id) const override;

    [[nodiscard]] bool doAccessToCard(int id, const std::string& pin) const override;
    [[nodiscard]] bool doWithdrawFromCard(int id, int amount) const override;
    [[nodiscard]] bool doDepositToCard(int id, int amount) const override;
    [[nodiscard]] bool doTransferBetweenCards(int fromCardId, int toCardId, int amount) const override;
    [[nodiscard]] bool doChangeCardPin(int id, const std::string& oldPin, const std::string& newPin) const override;

};
