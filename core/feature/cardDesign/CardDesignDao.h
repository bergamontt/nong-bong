#pragma once
#include <soci/soci.h>
#include "CardDesign.h"
#include "ICardDesignDao.h"

class CardDesignDao final : public ICardDesignDao
{
public:
    explicit CardDesignDao(soci::connection_pool& pool) : _pool(pool) {}
    CardDesignDao(const CardDesignDao&) = delete;
    CardDesignDao& operator=(const CardDesignDao&) = delete;
private:
    soci::connection_pool& _pool;

    CardDesign doGetById(int id) const override;
    std::vector<CardDesign> doGetAll() const override;
};