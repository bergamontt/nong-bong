#pragma once
#include "CardDesign.h"

class ICardDesignService{
public:
    std::optional<CardDesign> getCardDesignById(int id) const;
    std::vector<CardDesign> getAllCardDesigns() const;
    void createCardDesign(const CardDesign& cardDesign) const;
    void deleteAll() const;
    virtual ~ICardDesignService() = default;
private:
    virtual std::optional<CardDesign> doGetCardDesignById(int id) const = 0;
    virtual std::vector<CardDesign> doGetAllCardDesigns() const = 0;
    virtual void doCreateCardDesign(const CardDesign& cardDesign) const = 0;
    virtual void doDeleteAll() const = 0;
};

inline std::optional<CardDesign> ICardDesignService::getCardDesignById(const int id) const { return doGetCardDesignById(id); }
inline std::vector<CardDesign> ICardDesignService::getAllCardDesigns() const { return doGetAllCardDesigns(); }
inline void ICardDesignService::createCardDesign(const CardDesign& cardDesign) const { doCreateCardDesign(cardDesign);}
inline void ICardDesignService::deleteAll() const { doDeleteAll(); }