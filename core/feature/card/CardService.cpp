#include "CardService.h"

CardService::CardService(ICardDao &dao)
    : _cardDao(dao)
{}

std::optional<Card> CardService::doGetCardById(const int id) const
{
    return _cardDao.getById(id);
}

std::vector<Card> CardService::doGetAllActiveCardsByUserId(const int id) const
{
    return _cardDao.getByUserIdAndStatus(id, Card::Status::active);
}

std::vector<Card> CardService::doGetAllBlockedCardsByUserId(const int id) const
{
    return _cardDao.getByUserIdAndStatus(id, Card::Status::blocked);
}

std::vector<Card> CardService::doGetAllDeletedCardsByUserId(const int id) const
{
    return _cardDao.getByUserIdAndStatus(id, Card::Status::deleted);
}

std::vector<Card> CardService::doGetAllCardsByUserId(const int id) const
{
    return _cardDao.getByUserId(id);
}

void CardService::doCreateCard(const Card& card) const
{
    _cardDao.create(card);
}

void CardService::doUpdateCard(const Card& card) const
{
    _cardDao.update(card);
}

void CardService::doDeleteCardById(const int id) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();
    card.status = Card::Status::deleted;
    _cardDao.update(card);
}

bool CardService::doAccessToCard(const int id, const std::string &pin) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    const Card& card = optionalCard.value();
    // TODO: Add pin hashing and counter of incorrect accesses
    return card.pinHash == pin;
}

bool CardService::doChangeCardPin(const int id, const std::string& oldPin, const std::string& newPin) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();
    // TODO: Add pin hashing
    if (card.pinHash != oldPin || card.pinHash == newPin)
        return false;
    card.pinHash = newPin;
    updateCard(card);
    return true;
}