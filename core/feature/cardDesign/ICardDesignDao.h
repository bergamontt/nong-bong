#pragma once
#include <vector>
#include "CardDesign.h"

class ICardDesignDao
{
public:
    std::optional<CardDesign> getById(int id) const;
    std::vector<CardDesign> getAll() const;
    void create(const CardDesign& cardDesign) const;
    void deleteAll() const;
protected:
    ~ICardDesignDao() = default;
private:
    virtual std::optional<CardDesign> doGetById(int id) const = 0;
    virtual std::vector<CardDesign> doGetAll() const = 0;
    virtual void doCreate(const CardDesign& cardDesign) const = 0;
    virtual void doDeleteAll() const = 0;
};

inline std::optional<CardDesign> ICardDesignDao::getById(const int id) const
{
    return doGetById(id);
}

inline std::vector<CardDesign> ICardDesignDao::getAll() const
{
    return doGetAll();
}

inline void ICardDesignDao::create(const CardDesign& cardDesign) const {
    doCreate(cardDesign);
}

inline void ICardDesignDao::deleteAll() const {
    doDeleteAll();
}