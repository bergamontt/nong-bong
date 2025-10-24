#pragma once
#include <soci/soci.h>
#include "CardDesign.h"
#include "ICardDesignDao.h"

class CardDesignDao
{
private:
    soci::connection_pool& _pool;

    CardDesign doGetById(int id) const;
    std::vector<CardDesign> doGetAll() const;
public:
    CardDesignDao(soci::connection_pool& pool) : _pool(pool) {}
    CardDesignDao(const CardDesignDao&) = delete;
    CardDesignDao& operator=(const CardDesignDao&) = delete;
};