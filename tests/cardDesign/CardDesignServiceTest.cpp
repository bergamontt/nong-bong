#include "doctest.h"
#include "DBTestFixture.h"
#include "CardDesignDao.h"
#include "CardDesignService.h"
#include "CardDesignTestUtils.h"

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
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, cardDesign);
    }

    SUBCASE("createCardDesign should insert valid card design without author")
    {
        cardDesign.author.reset();
        CHECK_NOTHROW(service.createCardDesign(cardDesign));
        auto retrieved = service.getCardDesignById(cardDesign.id);
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, cardDesign);
    }

    SUBCASE("getCardDesignById should return design with given id if such design exists")
    {
        service.createCardDesign(cardDesign);
        auto retrieved = service.getCardDesignById(cardDesign.id);
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, cardDesign);
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
        REQUIRE_EQ(retrieved.size(), 1);
        assertCardDesignEquals(retrieved[0], cardDesign);
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