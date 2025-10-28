#pragma once
#include <vector>
#include "Card.h"

class ICardDao
{
public:
    std::optional<Card> getById(int id) const;
    std::vector<Card> getByUserId(int id) const;
    std::vector<Card> getByUserIdAndStatus(int id, Card::Status status) const;
    void update(const Card& card) const;
    void create(const Card& card) const;
protected:
    ~ICardDao() = default;
private:
    virtual std::optional<Card> doGetById(int id) const = 0;
    virtual std::vector<Card> doGetByUserId(int id) const = 0;
    virtual std::vector<Card> doGetByUserIdAndStatus(int id, Card::Status status) const;
    virtual void doUpdate(const Card& card) const = 0;
    virtual void doCreate(const Card& card) const = 0;
};

inline std::optional<Card> ICardDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<Card> ICardDao::getByUserId(const int id) const
{
    return doGetByUserId(id);
}

inline std::vector<Card> ICardDao::getByUserIdAndStatus(int id, Card::Status status) const
{
    return doGetByUserIdAndStatus(id, status);
}

inline void ICardDao::update(const Card& card) const
{
    doUpdate(card);
}

inline void ICardDao::create(const Card& card) const
{
    doCreate(card);
}