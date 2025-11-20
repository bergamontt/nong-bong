#include "doctest.h"
#include "DBTestFixture.h"
#include "CardDesignDao.h"
#include "CardDesignService.h"

TEST_CASE_FIXTURE(DBTestFixture, "CardDesign API Integration Test")
{
    CardDesignDao dao(pool());
    CardDesignService service(dao);

    CardDesign cardDesign;
    cardDesign.id = 1;
    cardDesign.name = "Name";
    cardDesign.author = "Author";
    cardDesign.imageRef = "img.png";

    SUBCASE("createCardDesign should insert valid card design with author")
    {
        CHECK_NOTHROW(service.createCardDesign(cardDesign));
        auto retrieved = service.getCardDesignById(cardDesign.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, cardDesign.id);
        CHECK_EQ(retrieved->name, cardDesign.name);
        CHECK_EQ(retrieved->imageRef, cardDesign.imageRef);
        CHECK(retrieved->author.has_value());
        CHECK_EQ(retrieved->author.value(), cardDesign.author.value());
    }

    SUBCASE("createCardDesign should insert valid card design without author")
    {
        cardDesign.author.reset();
        CHECK_NOTHROW(service.createCardDesign(cardDesign));
        auto retrieved = service.getCardDesignById(cardDesign.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, cardDesign.id);
        CHECK_EQ(retrieved->name, cardDesign.name);
        CHECK_EQ(retrieved->imageRef, cardDesign.imageRef);
        CHECK_FALSE(retrieved->author.has_value());
    }

    SUBCASE("getCardDesignById should return design with given id if such design exists")
    {
        service.createCardDesign(cardDesign);
        auto retrieved = service.getCardDesignById(cardDesign.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, cardDesign.id);
    }

    SUBCASE("getCardDesignById should return no value unless such design exists")
    {
        auto retrieved = service.getCardDesignById(cardDesign.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("getAllCardDesigns should return all inserted designs")
    {
        service.createCardDesign(cardDesign);
        auto retrieved = service.getAllCardDesigns();
        CHECK_EQ(retrieved.size(), 1);
        CHECK_EQ(retrieved[0].id, cardDesign.id);
        CHECK_EQ(retrieved[0].name, cardDesign.name);
        CHECK_EQ(retrieved[0].imageRef, cardDesign.imageRef);
        CHECK(retrieved[0].author.has_value());
        CHECK_EQ(retrieved[0].author.value(), cardDesign.author.value());
    }

    SUBCASE("deleteAll should remove all card designs")
    {
        service.createCardDesign(cardDesign);
        CHECK_NOTHROW(service.deleteAll());
        auto retrieved = service.getAllCardDesigns();
        CHECK(retrieved.empty());
        CHECK_FALSE(service.getCardDesignById(cardDesign.id).has_value());
    }

}