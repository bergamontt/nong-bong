#pragma once

class Card;

class CardValidator
{
public:
    static bool isTemporarilyBlocked(const Card& card);
};