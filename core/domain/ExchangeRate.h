#pragma once
#include <string>

class ExchangeRate
{
public:
    int id;
    std::string baseCurrency;
    std::string targetCurrency;
    double rate;
};