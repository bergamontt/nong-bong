#include "CardValidator.h"
#include "Card.h"

bool CardValidator::isTemporarilyBlocked(const Card& card)
{
    if (card.status != Card::Status::blocked || !card.blockedUntil.has_value())
        return false;
    const std::time_t now = std::time(nullptr);
    std::tm blockedUntilCopy = card.blockedUntil.value();
    const std::time_t blockedUntilTime = std::mktime(&blockedUntilCopy);
    return blockedUntilTime >= now;
}