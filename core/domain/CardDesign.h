#pragma once
#include <optional>
#include <string>

class CardDesign
{
public:
    int id;
    std::string name;
    std::optional<std::string> author;
    std::string imageRef;
};