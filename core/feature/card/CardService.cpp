#include "CardService.h"
#include "CardValidator.h"
#include "Hasher.h"
#include <string>
#include <iostream>

using namespace std;

CardService::CardService(ICardDao& cardDao, 
    IBankTransactionDao& transDao, IExchangeRateDao& rateDao)
    : _cardDao(cardDao), _transDao(transDao), _rateDao(rateDao)
{}

std::optional<Card> CardService::doGetCardById(const int id) const
{
    return _cardDao.getById(id);
}

std::optional<Card> CardService::doGetCardByNumber(const string& number) const
{
    return _cardDao.getByNumber(number);
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

int CardService::doGetCardSpendingsSince(const int cardId, const std::tm& time) const
{
    return _cardDao.getSpendingsSince(cardId, time);
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
    std::optional<Card> card = getCardById(id);
    if (!card.has_value())
        return;
    card->status = Card::Status::deleted;
    _cardDao.update(*card);
}

bool CardService::doAccessToCard(const int id, const std::string &pin)
{
    const std::optional<Card> optionalCard = getCardById(id);
    if (!optionalCard.has_value())
        return false;
    Card card = optionalCard.value();

    if (CardValidator::isTemporarilyBlocked(card))
        return false;

    const bool accessed = Hasher::verifyPin(pin, card.pinHash);
    if (accessed)
        card.failedAccessCount = 0;
    else handleFailedAccess(card);

    _cardDao.update(card);
    return accessed;
}

void CardService::handleFailedAccess(Card& card) const
{
    card.failedAccessCount++;
    if (card.failedAccessCount >= 3)
        blockCardTemporarily(card, 30 * 60);
}

void CardService::blockCardTemporarily(Card& card, const int seconds) const
{
    std::time_t now = std::time(nullptr);
    now += seconds;
    if (const auto tm_ptr = std::localtime(&now))
        card.blockedUntil = *tm_ptr;
    card.failedAccessCount = 0;
    card.status = Card::Status::blocked;
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