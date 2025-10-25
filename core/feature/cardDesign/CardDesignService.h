#pragma once
#include "CardDesign.h"
#include "ICardDesignDao.h"
#include "ICardDesignService.h"

class CardDesignService final : public ICardDesignService
{
public:
    explicit CardDesignService(ICardDesignDao& dao);
private:
    ICardDesignDao& dao;
    [[nodiscard]] std::optional<CardDesign> doGetCardDesignById(int id) const override;
    [[nodiscard]] std::vector<CardDesign> doGetAllCardDesigns() const override;
};
