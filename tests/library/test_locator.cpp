#include <catch2/catch_test_macros.hpp>
#include <campaign/locator.hpp>

using namespace campaign;

#define INIT_LOCATOR_TEST_DATA() \
    std::unique_ptr<Layout> layoutPtr = std::make_unique<Layout>(8); \
    initLayout(layoutPtr); \
    std::vector<uint8_t> data(layoutPtr->getDataSize()); \
    std::fill(data.begin(), data.end(), 0); \
    std::shared_ptr<Proxy> proxyPtr = std::make_shared<Proxy>(data.begin().base(), data.end().base()); \
    Locator locator(*layoutPtr, proxyPtr)

void initLayout(std::unique_ptr<Layout> &layoutPtr)
{
    layoutPtr->pushFlag("flag_first");

    layoutPtr->pushByte("byte_first");

    layoutPtr->pushFlag("flag_second");
    layoutPtr->pushFlag("flag_third");

    layoutPtr->pushDynamic("dynamic_int", 4);

    layoutPtr->pushFlag("flag_fourth");

    layoutPtr->pushByte("byte_second");

    layoutPtr->pushDynamic("dynamic_float", 4);
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
    REQUIRE_THROWS(locator.get<int32_t>("dynamic_int2"));
    
    REQUIRE_THROWS(locator.getByte("flag_first"));
    REQUIRE_THROWS(locator.getFlag("byte_first"));
    REQUIRE_THROWS(locator.get<int64_t>("dynamic_int"));
}

TEST_CASE("Locator Flag Access", "[locator]")
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

TEST_CASE("Locator Byte Access", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    REQUIRE(locator.getByte("byte_first") == 0);

    locator.setByte("byte_first", 127);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 0);

    locator.setByte("byte_second", 31);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 31);

    locator.setByte("byte_second", 0);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 0);
}

TEST_CASE("Locator Dynamic Access", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    REQUIRE(locator.get<int32_t>("dynamic_int") == 0);
    REQUIRE(locator.get<float>("dynamic_float") == 0.0f);

    locator.set<int32_t>("dynamic_int", -328);
    REQUIRE(locator.get<int32_t>("dynamic_int") == -328);
    REQUIRE(locator.get<float>("dynamic_float") == 0.0f);

    locator.set<float>("dynamic_float", 0.32f);
    REQUIRE(locator.get<int32_t>("dynamic_int") == -328);
    REQUIRE(locator.get<float>("dynamic_float") == 0.32f);
}

TEST_CASE("Locator Flag Token", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    auto aToken = locator.getFlagToken("flag_first");
    auto bToken = locator.getFlagToken("flag_second");

    REQUIRE(aToken.get() == false);
    REQUIRE(bToken.get() == false);
    REQUIRE(locator.getFlag("flag_first") == false);
    REQUIRE(locator.getFlag("flag_second") == false);

    aToken.set(true);
    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == false);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == false);
    
    bToken.set(true);
    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == true);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == true);

    bToken.set(false);
    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == false);
    REQUIRE(locator.getFlag("flag_first") == true);
    REQUIRE(locator.getFlag("flag_second") == false);
}

TEST_CASE("Locator Byte Token", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    auto aToken = locator.getByteToken("byte_first");
    auto bToken = locator.getByteToken("byte_second");

    REQUIRE(aToken.get() == 0);
    REQUIRE(bToken.get() == 0);
    REQUIRE(locator.getByte("byte_first") == 0);
    REQUIRE(locator.getByte("byte_second") == 0);

    aToken.set(127);
    REQUIRE(aToken.get() == 127);
    REQUIRE(bToken.get() == 0);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 0);

    bToken.set(31);
    REQUIRE(aToken.get() == 127);
    REQUIRE(bToken.get() == 31);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 31);

    bToken.set(0);
    REQUIRE(aToken.get() == 127);
    REQUIRE(bToken.get() == 0);
    REQUIRE(locator.getByte("byte_first") == 127);
    REQUIRE(locator.getByte("byte_second") == 0);
}

TEST_CASE("Locator Dynamic Token", "[locator]")
{
    INIT_LOCATOR_TEST_DATA();

    auto aToken = locator.getDynamicToken<int32_t>("dynamic_int");
    auto bToken = locator.getDynamicToken<float>("dynamic_float");

    REQUIRE(aToken.get() == 0);
    REQUIRE(bToken.get() == 0.0f);
    REQUIRE(locator.get<int32_t>("dynamic_int") == 0);
    REQUIRE(locator.get<float>("dynamic_float") == 0.0f);

    aToken.set(-328);
    REQUIRE(aToken.get() == -328);
    REQUIRE(bToken.get() == 0.0f);
    REQUIRE(locator.get<int32_t>("dynamic_int") == -328);
    REQUIRE(locator.get<float>("dynamic_float") == 0.0f);

    bToken.set(0.32f);
    REQUIRE(aToken.get() == -328);
    REQUIRE(bToken.get() == 0.32f);
    REQUIRE(locator.get<int32_t>("dynamic_int") == -328);
    REQUIRE(locator.get<float>("dynamic_float") == 0.32f);
}

TEST_CASE("Locator Flag Notification", "[proxy]")
{
    int callCount = 0;
    auto callback = [&](bool previous, bool current)
    {
        callCount++;
    };

    INIT_LOCATOR_TEST_DATA();

    auto token = locator.registerFlagCallback("flag_first", callback);

    REQUIRE(callCount == 0);
    locator.setFlag("flag_first", true);
    REQUIRE(callCount == 1);
    locator.setFlag("flag_second", true);
    REQUIRE(callCount == 1);
    locator.setFlag("flag_second", true);
    REQUIRE(callCount == 1);
    locator.setFlag("flag_first", false);
    REQUIRE(callCount == 2);
    locator.setFlag("flag_first", false);
    REQUIRE(callCount == 2);
    locator.setFlag("flag_first", true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    REQUIRE(callCount == 0);
    locator.setFlag("flag_first", true);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_first", false);
    REQUIRE(callCount == 0);

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    token = locator.registerFlagCallback("flag_third", callback);

    REQUIRE(callCount == 0);
    locator.setFlag("flag_first", true);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_first", false);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_second", true);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_second", false);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_fourth", true);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_fourth", false);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_third", true);
    REQUIRE(callCount == 1);
    locator.setFlag("flag_third", false);
    REQUIRE(callCount == 2);
    locator.setFlag("flag_third", false);
    REQUIRE(callCount == 2);
    locator.setFlag("flag_third", true);
    REQUIRE(callCount == 3);
    locator.setFlag("flag_third", true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    REQUIRE(callCount == 0);
    locator.setFlag("flag_third", true);
    REQUIRE(callCount == 0);
    locator.setFlag("flag_third", false);
    REQUIRE(callCount == 0);
}

TEST_CASE("Locator Byte Notification", "[proxy]")
{
    int callCount = 0;
    auto callback = [&](uint8_t previous, uint8_t current)
    {
        callCount++;
    };

    INIT_LOCATOR_TEST_DATA();

    auto token = locator.registerByteCallback("byte_first", callback);

    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 1);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 1);
    locator.setByte("byte_first", 25);
    REQUIRE(callCount == 2);
    locator.setByte("byte_first", 0);
    REQUIRE(callCount == 3);
    locator.setByte("byte_second", 1);
    REQUIRE(callCount == 3);
    locator.setByte("byte_second", 36);
    REQUIRE(callCount == 3);
    locator.setByte("byte_first", 36);
    REQUIRE(callCount == 4);

    token.unregister();

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 25);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 0);
    REQUIRE(callCount == 0);
    locator.setByte("byte_second", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_second", 36);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 36);
    REQUIRE(callCount == 0);

    token = locator.registerByteCallback("byte_second", callback);

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 25);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 0);
    REQUIRE(callCount == 0);
    locator.setByte("byte_second", 1);
    REQUIRE(callCount == 1);
    locator.setByte("byte_second", 36);
    REQUIRE(callCount == 2);
    locator.setByte("byte_first", 36);
    REQUIRE(callCount == 2);

    token.unregister();

    callCount = 0;
    std::fill(data.begin(), data.end(), 0);

    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 25);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 0);
    REQUIRE(callCount == 0);
    locator.setByte("byte_second", 1);
    REQUIRE(callCount == 0);
    locator.setByte("byte_second", 36);
    REQUIRE(callCount == 0);
    locator.setByte("byte_first", 36);
    REQUIRE(callCount == 0);
}