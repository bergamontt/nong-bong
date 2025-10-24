#pragma once
#include <soci/soci.h>
#include "Card.h"
#include "ICardDao.h"

class CardDao
{
private:
    soci::connection_pool& _pool;

    Card doGetById(int id) const;
    std::vector<Card> doGetByUserId(int id) const;
    void doUpdate(const Card& card) const;
    void doCreate(const Card& card) const;
public:
    CardDao(soci::connection_pool& pool) : _pool(pool) {}
    CardDao(const CardDao&) = delete;
    CardDao& operator=(const CardDao&)& = delete;
};