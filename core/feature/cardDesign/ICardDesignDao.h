#pragma once
#include <vector>
#include "CardDesign.h"

class ICardDesignDao
{
public:
    CardDesign getById(int id) const;
    std::vector<CardDesign> getAll() const;
protected:
    ~ICardDesignDao() = default;
private:
    virtual CardDesign doGetById(int id) const = 0;
    virtual std::vector<CardDesign> doGetAll() const = 0;
};

inline CardDesign ICardDesignDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<CardDesign> ICardDesignDao::getAll() const
{
    return doGetAll();
}