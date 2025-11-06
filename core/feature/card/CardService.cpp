#include "CardService.h"

#include <iostream>
#include <qlogging.h>

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

bool CardService::doAccessToCard(const int id, const std::string &pin) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    const Card& card = optionalCard.value();

    //FOR TEST
    std::cout << card.cardNumber << std::endl;
    std::cout << Hasher::hashPin("4321") << std::endl << Hasher::hashPin(pin) << std::endl << card.pinHash << std::endl;
    bool a = Hasher::verifyPin("4321", Hasher::hashPin(pin));
    std::cout << "a = " << a << std::endl;
    bool b = Hasher::verifyPin(pin, card.pinHash);
    std::cout << "b = " << b << std::endl;
    bool c = Hasher::verifyPin("4321", card.pinHash);
    std::cout << "c = " << c << std::endl;
    bool d = Hasher::verifyPin("4321", "$argon2id$v=19$m=65536,t=2,p=1$UU9EUU5HZUpwbGpNcFpGRQ$NWWojBCLZX63RgIHm3lWsOGUL7sti+8O+N/CQDMiJEE");
    std::cout << "d = " << d << std::endl;
    return b;
}

bool CardService::doChangeCardPin(const int id, const std::string& oldPin, const std::string& newPin) const
{
    const std::optional<Card> optionalCard = getCardById(id);
    Card card = optionalCard.value();
    if ( !(Hasher::verifyPin(oldPin, card.pinHash)) || Hasher::verifyPin(newPin, card.pinHash))
        return false;
    card.pinHash = Hasher::hashPin(newPin);
    updateCard(card);
    return true;
}