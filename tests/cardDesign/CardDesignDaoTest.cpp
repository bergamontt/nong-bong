#include "doctest.h"
#include "DBTestFixture.h"
#include "CardDesignDao.h"
#include "EntityTestUtils.h"

TEST_CASE_FIXTURE(DBTestFixture, "CardDesign API test")
{
    const CardDesignDao dao(pool());

    CardDesign design;
    design.id = 1;
    design.name = "Name";
    design.author = "Author";
    design.imageRef = "img001.png";

    SUBCASE("Create should insert valid card design")
    {
        CHECK_NOTHROW(dao.create(design));
        auto retrieved = dao.getById(design.id);
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, design);
    }

    SUBCASE("Create should insert valid card design without an author")
    {
        design.author.reset();
        CHECK_NOTHROW(dao.create(design));
        auto retrieved = dao.getById(design.id);
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, design);
    }

    SUBCASE("Get by id should return card design with the given id if such design exists")
    {
        dao.create(design);
        auto retrieved = dao.getById(design.id);
        REQUIRE(retrieved.has_value());
        assertCardDesignEquals(*retrieved, design);
    }

    SUBCASE("Get by id should return no value unless such design exists")
    {
        auto retrieved = dao.getById(design.id);
        CHECK_FALSE(retrieved.has_value());
    }

    SUBCASE("Get all should return all designs")
    {
        dao.create(design);

        CardDesign another;
        another.id = 2;
        another.name = "Name2";
        another.author = "Author2";
        another.imageRef = "img002.png";
        dao.create(another);

        auto all = dao.getAll();
        REQUIRE_EQ(all.size(), 2);
        assertCardDesignEquals(all[0], design);
        assertCardDesignEquals(all[1], another);
    }

    SUBCASE("Delete all should remove all designs")
    {
        dao.create(design);
        CHECK_NOTHROW(dao.deleteAll());
        auto all = dao.getAll();
        CHECK_EQ(all.size(), 0);
    }
}