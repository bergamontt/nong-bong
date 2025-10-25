#pragma once
#include <soci/soci.h>
#include "Card.h"
#include "ICardDao.h"

class CardDao final : public ICardDao
{
public:
    explicit CardDao(soci::connection_pool& pool) : _pool(pool) {}
    CardDao(const CardDao&) = delete;
    CardDao& operator=(const CardDao&)& = delete;
private:
    soci::connection_pool& _pool;

    Card doGetById(int id) const override;
    std::vector<Card> doGetByUserId(int id) const override;
    void doUpdate(const Card& card) const override;
    void doCreate(const Card& card) const override;
};