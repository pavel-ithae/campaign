#include <catch2/catch_test_macros.hpp>
#include <campaign/data_locator.hpp>

using namespace campaign;

#define INIT_LOCATOR_TEST_DATA() \
    std::unique_ptr<DataLayout> layoutPtr; \
    std::shared_ptr<DataSource> sourcePtr; \
    initTestData(layoutPtr, sourcePtr); \
    DataLocator locator(*layoutPtr, sourcePtr);

void initTestData(std::unique_ptr<DataLayout> &layoutPtr, std::shared_ptr<DataSource> &source)
{
    layoutPtr = std::make_unique<DataLayout>(7);

    layoutPtr->pushFlag("flag_first");

    layoutPtr->pushByte("byte_first");

    layoutPtr->pushFlag("flag_second");
    layoutPtr->pushFlag("flag_third");

    layoutPtr->pushDynamic("dynamic_int", 4);

    layoutPtr->pushFlag("flag_fourth");

    layoutPtr->pushByte("byte_second");

    source = std::make_shared<DataSource>(layoutPtr->getDataSize());
    source->init();
}

TEST_CASE("Locator Init", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    REQUIRE_NOTHROW(locator.getFlag("flag_first"));

    REQUIRE_NOTHROW(locator.getByte("byte_first"));

    REQUIRE_NOTHROW(locator.getFlag("flag_second"));
    REQUIRE_NOTHROW(locator.getFlag("flag_third"));

    REQUIRE_NOTHROW(locator.get<int32_t>("dynamic_int"));

    REQUIRE_NOTHROW(locator.getFlag("flag_fourth"));

    REQUIRE_NOTHROW(locator.getByte("byte_second"));

    REQUIRE_THROWS(locator.getFlag("flag_fifth"));
    REQUIRE_THROWS(locator.getByte("byte_third"));
    REQUIRE_THROWS(locator.get<int32_t>("dynamic_float"));
    
    REQUIRE_THROWS(locator.getByte("flag_first"));
    REQUIRE_THROWS(locator.getFlag("byte_first"));
    REQUIRE_THROWS(locator.get<int64_t>("dynamic_int"));
}

TEST_CASE("Locator Set", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    REQUIRE(locator.getFlag("flag_first") == false);

    locator.setFlag("flag_first", true);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == false);

    locator.setFlag("flag_second", true);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == true);

    locator.setFlag("flag_second", false);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == false);
}