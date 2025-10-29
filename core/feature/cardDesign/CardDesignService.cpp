#include "CardDesignService.h"

CardDesignService::CardDesignService(ICardDesignDao &dao) :
    _cardDesignDao(dao)
{}

std::optional<CardDesign> CardDesignService::doGetCardDesignById(int id) const {
    return  _cardDesignDao.getById(id);
}

std::vector<CardDesign> CardDesignService::doGetAllCardDesigns() const{
    return _cardDesignDao.getAll();
}
