#include "CardService.h"
#include <string>
#include <iostream>
#include "Hasher.h"

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

int CardService::doGetCardSpendingsSince(int cardId, std::tm time) const
{
    std::optional<Card> card = _cardDao.getById(cardId);
    std::string& cardCurrCode = card.value().currencyCode;
    std::vector<BankTransaction> transactions = _transDao.getByFromCardId(cardId);
    std::vector<ExchangeRate> rates = _rateDao.getAll();
    int res = 0;
    time_t t = std::mktime(&time);
    for (auto& trans : transactions) 
    {
        time_t transTime = std::mktime(&trans.createdAt);
        if (transTime < t)
            continue;
        std::string transCurrCode = trans.currencyCode;
        int amount = trans.amount;
        if (transCurrCode != cardCurrCode)
        {
            for (auto& rate : rates) 
            {
                if (rate.baseCurrency == transCurrCode
                    && rate.targetCurrency == cardCurrCode) 
                {
                    amount *= rate.rate;
                }
            }
        }
        res += amount;
    }
    return res;
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