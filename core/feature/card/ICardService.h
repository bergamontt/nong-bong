#pragma once
#include <vector>
#include "Card.h"

class ICardService
{
public:
    [[nodiscard]] std::optional<Card> getCardById(int id) const;

    [[nodiscard]] std::vector<Card> getAllActiveCardsByUserId(int id) const;
    [[nodiscard]] std::vector<Card> getAllBlockedCardsByUserId(int id) const;
    [[nodiscard]] std::vector<Card> getAllDeletedCardsByUserId(int id) const;
    [[nodiscard]] std::vector<Card> getAllCardsByUserId(int id) const;

    void createCard(const Card& card) const;
    void updateCard(const Card& card) const;
    void deleteCardById(int id) const;

    [[nodiscard]] bool accessToCard(int id, const std::string& pin) const;
    [[nodiscard]] bool changeCardPin(int id, const std::string& oldPin, const std::string& newPin) const;

protected:
    ~ICardService() = default;

private:
    [[nodiscard]] virtual std::optional<Card> doGetCardById(int id) const = 0;

    [[nodiscard]] virtual std::vector<Card> doGetAllActiveCardsByUserId(int id) const = 0;
    [[nodiscard]] virtual std::vector<Card> doGetAllBlockedCardsByUserId(int id) const = 0;
    [[nodiscard]] virtual std::vector<Card> doGetAllDeletedCardsByUserId(int id) const = 0;
    [[nodiscard]] virtual std::vector<Card> doGetAllCardsByUserId(int id) const = 0;

    virtual void doCreateCard(const Card& card) const = 0;
    virtual void doUpdateCard(const Card& card) const = 0;
    virtual void doDeleteCardById(int id) const = 0;

    [[nodiscard]] virtual bool doAccessToCard(int id, const std::string& pin) const = 0;
    [[nodiscard]] virtual bool doChangeCardPin(int id, const std::string& oldPin, const std::string& newPin) const = 0;
};

inline std::optional<Card> ICardService::getCardById(const int id) const{ return doGetCardById(id); }

inline std::vector<Card> ICardService::getAllActiveCardsByUserId(const int id) const{ return doGetAllActiveCardsByUserId(id); }
inline std::vector<Card> ICardService::getAllBlockedCardsByUserId(const int id) const{ return doGetAllBlockedCardsByUserId(id); }
inline std::vector<Card> ICardService::getAllDeletedCardsByUserId(const int id) const{ return doGetAllDeletedCardsByUserId(id); }
inline std::vector<Card> ICardService::getAllCardsByUserId(const int id) const{ return doGetAllCardsByUserId(id); }

inline void ICardService::createCard(const Card& card) const{ doCreateCard(card); }
inline void ICardService::updateCard(const Card& card) const{ doUpdateCard(card);}
inline void ICardService::deleteCardById(const int id) const { doDeleteCardById(id); }

inline bool ICardService::accessToCard(const int id, const std::string& pin) const { return doAccessToCard(id, pin); }
inline bool ICardService::changeCardPin(const int id, const std::string& oldPin, const std::string& newPin) const { return doChangeCardPin(id, oldPin, newPin); }
