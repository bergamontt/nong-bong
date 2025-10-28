#pragma once
#include "CardDesign.h"
#include "ICardDesignDao.h"
#include "ICardDesignService.h"

class CardDesignService final : public ICardDesignService
{
public:
    explicit CardDesignService(ICardDesignDao& dao);
    CardDesignService(const CardDesignService&) = delete;
    CardDesignService& operator=(const CardDesignService&)& = delete;

private:
    ICardDesignDao& _cardDesignDao;

    std::optional<CardDesign> doGetCardDesignById(int id) const override;
    std::vector<CardDesign> doGetAllCardDesigns() const override;
};
