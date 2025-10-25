#pragma once
#include "CardDesign.h"
#include "ICardDesignDao.h"
#include "ICardDesignService.h"

class CardDesignService final : public ICardDesignService{
    std::shared_ptr<ICardDesignDao> dao;

    explicit CardDesignService(std::shared_ptr<ICardDesignDao> dao);

    [[nodiscard]] std::optional<CardDesign> doGetCardDesignById(int id) const override;
    [[nodiscard]] std::vector<CardDesign> doGetAllCardDesigns() const override;
};
