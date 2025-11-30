#pragma once
#include <string>
#include <vector>
#include "Card.h"

class ICardService
{
public:
    std::optional<Card> getCardById(int id) const;
    std::optional<Card> getCardByNumber(const std::string& number) const;

    std::vector<Card> getAllActiveCardsByUserId(int id) const;
    std::vector<Card> getAllBlockedCardsByUserId(int id) const;
    std::vector<Card> getAllDeletedCardsByUserId(int id) const;
    std::vector<Card> getAllCardsByUserId(int id) const;
    int getCardSpendingsSince(int cardId, const std::tm &time) const;

    void createCard(const Card& card) const;
    void updateCard(const Card& card) const;
    void deleteCardById(int id) const;

    bool accessToCard(int id, const std::string& pin);
    bool changeCardPin(int id, const std::string& oldPin, const std::string& newPin) const;

protected:
    ~ICardService() = default;

private:
    virtual std::optional<Card> doGetCardById(int id) const = 0;
    virtual std::optional<Card> doGetCardByNumber(const std::string& number) const = 0;

    virtual std::vector<Card> doGetAllActiveCardsByUserId(int id) const = 0;
    virtual std::vector<Card> doGetAllBlockedCardsByUserId(int id) const = 0;
    virtual std::vector<Card> doGetAllDeletedCardsByUserId(int id) const = 0;
    virtual std::vector<Card> doGetAllCardsByUserId(int id) const = 0;
    virtual int doGetCardSpendingsSince(int cardId, const std::tm& time) const = 0;

    virtual void doCreateCard(const Card& card) const = 0;
    virtual void doUpdateCard(const Card& card) const = 0;
    virtual void doDeleteCardById(int id) const = 0;

    virtual bool doAccessToCard(int id, const std::string& pin) = 0;
    virtual bool doChangeCardPin(int id, const std::string& oldPin, const std::string& newPin) const = 0;
};

inline std::optional<Card> ICardService::getCardById(const int id) const
{
    return doGetCardById(id);
}

inline std::optional<Card> ICardService::getCardByNumber(const std::string& number) const
{
    return doGetCardByNumber(number);
}

inline std::vector<Card> ICardService::getAllActiveCardsByUserId(const int id) const
{
    return doGetAllActiveCardsByUserId(id);
}

inline std::vector<Card> ICardService::getAllBlockedCardsByUserId(const int id) const
{
    return doGetAllBlockedCardsByUserId(id);
}

inline std::vector<Card> ICardService::getAllDeletedCardsByUserId(const int id) const
{
    return doGetAllDeletedCardsByUserId(id);
}

inline std::vector<Card> ICardService::getAllCardsByUserId(const int id) const
{
    return doGetAllCardsByUserId(id);
}

inline int ICardService::getCardSpendingsSince(const int cardId, const std::tm &time) const
{
    return doGetCardSpendingsSince(cardId, time);
}

inline void ICardService::createCard(const Card& card) const
{
    doCreateCard(card);
}

inline void ICardService::updateCard(const Card& card) const
{
    doUpdateCard(card);
}

inline void ICardService::deleteCardById(const int id) const
{
    doDeleteCardById(id);
}

inline bool ICardService::accessToCard(const int id, const std::string& pin)
{
    return doAccessToCard(id, pin);
}

inline bool ICardService::changeCardPin(const int id, const std::string& oldPin, const std::string& newPin) const
{
    return doChangeCardPin(id, oldPin, newPin);
}
