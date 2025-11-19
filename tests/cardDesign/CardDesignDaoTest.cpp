#include "doctest.h"
#include "DBTestFixture.h"
#include "CardDesignDao.h"

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
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->name, design.name);
        CHECK_EQ(retrieved->author.value(), design.author.value());
        CHECK_EQ(retrieved->imageRef, design.imageRef);
    }

    SUBCASE("Create should insert valid card design without an author")
    {
        design.author.reset();
        CHECK_NOTHROW(dao.create(design));
        auto retrieved = dao.getById(design.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->name, design.name);
        CHECK_FALSE(retrieved->author.has_value());
        CHECK_EQ(retrieved->imageRef, design.imageRef);
    }

    SUBCASE("Get by id should return card design with the given id if such design exists")
    {
        dao.create(design);
        auto retrieved = dao.getById(design.id);
        CHECK(retrieved.has_value());
        CHECK_EQ(retrieved->id, design.id);
        CHECK_EQ(retrieved->name, design.name);
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
        another.name = "Name2";
        another.author = "Author2";
        another.imageRef = "img002.png";
        dao.create(another);

        auto all = dao.getAll();
        CHECK_EQ(all.size(), 2);
        CHECK_EQ(all[0].name, "Name");
        CHECK_EQ(all[1].name, "Name2");
    }

    SUBCASE("Delete all should remove all designs")
    {
        dao.create(design);
        CHECK_NOTHROW(dao.deleteAll());
        auto all = dao.getAll();
        CHECK_EQ(all.size(), 0);
    }
}