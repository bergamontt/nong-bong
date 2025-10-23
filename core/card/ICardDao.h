#pragma once
#include <vector>
#include "Card.h"

class ICardDao
{
public:
    Card getById(int id) const;
    std::vector<Card> getByUserId(int id) const;
    void update(const Card& card) const;
    void create(const Card& card) const;
    virtual ~ICardDao() = default;
private:
    virtual Card doGetById(int id) const = 0;
    virtual std::vector<Card> doGetByUserId(int id) const = 0;
    virtual void doUpdate(const Card& card) const = 0;
    virtual void doCreate(const Card& card) const = 0;
};

inline Card ICardDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<Card> ICardDao::getByUserId(const int id) const
{
    return doGetByUserId(id);
}

inline void ICardDao::update(const Card& card) const
{
    doUpdate(card);
}

inline void ICardDao::create(const Card& card) const
{
    doCreate(card);
}