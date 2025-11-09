#include "CardService.h"
#include <iostream>
#include "Hasher.h"

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
    Card newCard = card;
    newCard.pinHash = Hasher::hashPin(card.pinHash);
    _cardDao.create(newCard);
}

void CardService::doUpdateCard(const Card& card) const
{
    Card newCard = card;
    newCard.pinHash = Hasher::hashPin(card.pinHash);
    _cardDao.update(newCard);
}

void CardService::doDeleteCardById(const int id) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();
    card.status = Card::Status::deleted;
    _cardDao.update(card);
}

bool CardService::doAccessToCard(const int id, const std::string &pin)
{
    std::time_t now = std::time(nullptr);
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();

    if (card.status == Card::Status::blocked) {
        std::time_t blockedUntilTime = std::mktime(&card.blockedUntil.value());
        if (blockedUntilTime >= now) {
            return false;
        }
    }
    const bool accept = Hasher::verifyPin(pin, card.pinHash);
    if (!accept) {
        if (++card.failedAccessCount==3) {
            now += 30 * 60;
            std::tm* tm_ptr = std::localtime(&now);
            card.blockedUntil = *tm_ptr;
            card.failedAccessCount = 0;
            card.status = Card::Status::blocked;
        }
    } else {
        card.failedAccessCount=0;
    }

    _cardDao.update(card);
    return accept;
}

bool CardService::doChangeCardPin(const int id, const std::string& oldPin, const std::string& newPin) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();
    if ( !Hasher::verifyPin(oldPin, card.pinHash) || Hasher::verifyPin(newPin, card.pinHash))
        return false;
    card.pinHash = Hasher::hashPin(newPin);
    _cardDao.updatePin(card);
    return true;
}