#pragma once
#include "CardDesign.h"
#include "ICardDesignDao.h"
#include "ICardDesignService.h"

class CardDesignService : public ICardDesignService{
    std::shared_ptr<ICardDesignDao> dao;

    explicit CardDesignService(std::shared_ptr<ICardDesignDao> dao);

    std::optional<CardDesign> doGetCardDesignById(int id) const override;
    std::vector<CardDesign> doGetAllCardDesigns() const override;
};
