#include <catch2/catch_test_macros.hpp>
#include <campaign/data_source.hpp>
#include <campaign/testing/event_test_helper.hpp>

using namespace campaign;
using namespace campaign::testing;

TEST_CASE("Source Init Empty", "[source]")
{
    DataSource source(4);

    REQUIRE(source.getSize() == 4);

    source.init();

    REQUIRE(source.getByte(0) == 0x0);
    REQUIRE(source.getByte(1) == 0x0);
    REQUIRE(source.getByte(2) == 0x0);
    REQUIRE(source.getByte(3) == 0x0);

    source = DataSource(8);

    REQUIRE(source.getSize() == 8);

    source.init();

    REQUIRE(source.getByte(0) == 0x0);
    REQUIRE(source.getByte(1) == 0x0);
    REQUIRE(source.getByte(2) == 0x0);
    REQUIRE(source.getByte(3) == 0x0);
    REQUIRE(source.getByte(4) == 0x0);
    REQUIRE(source.getByte(5) == 0x0);
    REQUIRE(source.getByte(6) == 0x0);
    REQUIRE(source.getByte(7) == 0x0);
}

TEST_CASE("Source Flags", "[source]")
{
    DataSource source(4);

    source.init();

    REQUIRE(source.getFlag(0, 0x1 << 0) == false);
    REQUIRE(source.getFlag(0, 0x1 << 1) == false);
    REQUIRE(source.getFlag(0, 0x1 << 2) == false);
    REQUIRE(source.getFlag(0, 0x1 << 3) == false);
    REQUIRE(source.getFlag(1, 0x1 << 0) == false);
    REQUIRE(source.getFlag(1, 0x1 << 1) == false);
    REQUIRE(source.getFlag(1, 0x1 << 2) == false);
    REQUIRE(source.getFlag(1, 0x1 << 3) == false);

    source.setFlag(0, 0x1 << 0, true);
    source.setFlag(0, 0x1 << 3, true);

    REQUIRE(source.getFlag(0, 0x1 << 0) == true);
    REQUIRE(source.getFlag(0, 0x1 << 1) == false);
    REQUIRE(source.getFlag(0, 0x1 << 2) == false);
    REQUIRE(source.getFlag(0, 0x1 << 3) == true);
    REQUIRE(source.getFlag(1, 0x1 << 0) == false);
    REQUIRE(source.getFlag(1, 0x1 << 1) == false);
    REQUIRE(source.getFlag(1, 0x1 << 2) == false);
    REQUIRE(source.getFlag(1, 0x1 << 3) == false);

    source.setFlag(1, 0x1 << 1, true);
    source.setFlag(1, 0x1 << 2, true);

    REQUIRE(source.getFlag(0, 0x1 << 0) == true);
    REQUIRE(source.getFlag(0, 0x1 << 1) == false);
    REQUIRE(source.getFlag(0, 0x1 << 2) == false);
    REQUIRE(source.getFlag(0, 0x1 << 3) == true);
    REQUIRE(source.getFlag(1, 0x1 << 0) == false);
    REQUIRE(source.getFlag(1, 0x1 << 1) == true);
    REQUIRE(source.getFlag(1, 0x1 << 2) == true);
    REQUIRE(source.getFlag(1, 0x1 << 3) == false);
}

TEST_CASE("Source Bytes", "[source]")
{
    DataSource source(4);

    source.init();

    source.setByte(0, 12);
    source.setByte(3, 32);

    REQUIRE(source.getByte(0) == 12);
    REQUIRE(source.getByte(1) == 0);
    REQUIRE(source.getByte(2) == 0);
    REQUIRE(source.getByte(3) == 32);

    source.setByte(1, 64);
    source.setByte(2, 255);

    REQUIRE(source.getByte(0) == 12);
    REQUIRE(source.getByte(1) == 64);
    REQUIRE(source.getByte(2) == 255);
    REQUIRE(source.getByte(3) == 32);
}

TEST_CASE("Source Dynamic", "[source]")
{
    DataSource source(8);

    source.init();

    source.set<uint32_t>(0, 255);

    REQUIRE(source.get<uint32_t>(0) == 255);
    REQUIRE(source.getByte(0) == 255);
    REQUIRE(source.getByte(1) == 0);
    REQUIRE(source.getByte(2) == 0);
    REQUIRE(source.getByte(3) == 0);
    REQUIRE(source.getByte(4) == 0);
    REQUIRE(source.getByte(5) == 0);
    REQUIRE(source.getByte(6) == 0);
    REQUIRE(source.getByte(7) == 0);

    source.set<uint32_t>(0, 256);
    REQUIRE(source.get<uint32_t>(0) == 256);
    REQUIRE(source.getByte(0) == 0);
    REQUIRE(source.getByte(1) == 1);
    REQUIRE(source.getByte(2) == 0);
    REQUIRE(source.getByte(3) == 0);
    REQUIRE(source.getByte(4) == 0);
    REQUIRE(source.getByte(5) == 0);
    REQUIRE(source.getByte(6) == 0);
    REQUIRE(source.getByte(7) == 0);

    source.set<uint32_t>(3, 255);
    REQUIRE(source.get<uint32_t>(3) == 255);
    REQUIRE(source.getByte(0) == 0);
    REQUIRE(source.getByte(1) == 1);
    REQUIRE(source.getByte(2) == 0);
    REQUIRE(source.getByte(3) == 255);
    REQUIRE(source.getByte(4) == 0);
    REQUIRE(source.getByte(5) == 0);
    REQUIRE(source.getByte(6) == 0);
    REQUIRE(source.getByte(7) == 0);
}

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
TEST_CASE("Source Bounds Checking", "[source]")
{
    DataSource source(4);

    CHECK_NOTHROW([&]()
                 { return source.getFlag(3, 0x1); }());
    CHECK_NOTHROW([&]()
                 { return source.getByte(3) == 0x0; }());
    CHECK_NOTHROW([&]()
                 { return source.get<uint32_t>(0); }());

    CHECK_THROWS([&]()
                 { return source.getFlag(4, 0x1); }());
    CHECK_THROWS([&]()
                 { return source.getByte(4) == 0x0; }());
    CHECK_THROWS([&]()
                 { return source.get<uint32_t>(1); }());
    CHECK_THROWS([&]()
                 { return source.get<uint64_t>(0); }());
}
#endif

TEST_CASE("Source Flag Notification", "[source]")
{
    int callCount = 0;
    auto callback = [&](bool previous, bool current)
    {
        callCount++;
    };

    DataSource source(4);
    source.init();

    auto token = source.registerFlagCallback(0, 0x1 << 0, callback);

    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 1);
    source.setFlag(0, 0x1 << 1, true);
    REQUIRE(callCount == 1);
    source.setFlag(0, 0x1 << 1, false);
    REQUIRE(callCount == 1);
    source.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 2);
    source.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 2);
    source.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 0);

    callCount = 0;
    source.init();

    token = source.registerFlagCallback(2, 0x1 << 4, callback);

    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 4, true);
    REQUIRE(callCount == 0);
    source.setFlag(0, 0x1 << 4, false);
    REQUIRE(callCount == 0);
    source.setFlag(2, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    source.setFlag(2, 0x1 << 0, false);
    REQUIRE(callCount == 0);
    source.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 1);
    source.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 2);
    source.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 2);
    source.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 3);
    source.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 0);
    source.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 0);
}

TEST_CASE("Source Byte Notification", "[source]")
{
    int callCount = 0;
    auto callback = [&](uint8_t previous, uint8_t current)
    {
        callCount++;
    };

    DataSource source(4);
    source.init();

    auto token = source.registerByteCallback(0, callback);

    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 1);
    source.setByte(0, 1);
    REQUIRE(callCount == 1);
    source.setByte(0, 25);
    REQUIRE(callCount == 2);
    source.setByte(0, 0);
    REQUIRE(callCount == 3);
    source.setByte(1, 1);
    REQUIRE(callCount == 3);
    source.setByte(1, 36);
    REQUIRE(callCount == 3);
    source.setByte(0, 36);
    REQUIRE(callCount == 4);

    token.unregister();

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 0);
    source.setByte(0, 25);
    REQUIRE(callCount == 0);
    source.setByte(0, 0);
    REQUIRE(callCount == 0);
    source.setByte(1, 1);
    REQUIRE(callCount == 0);
    source.setByte(1, 36);
    REQUIRE(callCount == 0);
    source.setByte(0, 36);
    REQUIRE(callCount == 0);

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 0);
    source.setByte(0, 25);
    REQUIRE(callCount == 0);
    source.setByte(0, 0);
    REQUIRE(callCount == 0);

    token = source.registerByteCallback(3, callback);

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 0);
    source.setByte(0, 1);
    REQUIRE(callCount == 0);
    source.setByte(1, 25);
    REQUIRE(callCount == 0);
    source.setByte(1, 0);
    REQUIRE(callCount == 0);
    source.setByte(3, 1);
    REQUIRE(callCount == 1);
    source.setByte(3, 36);
    REQUIRE(callCount == 2);
    source.setByte(0, 36);
    REQUIRE(callCount == 2);

    token.unregister();

    callCount = 0;
    source.init();

    REQUIRE(callCount == 0);
    source.setByte(3, 1);
    REQUIRE(callCount == 0);
    source.setByte(3, 25);
    REQUIRE(callCount == 0);
    source.setByte(3, 0);
    REQUIRE(callCount == 0);
}

TEST_CASE("Source Copy Init", "[source]")
{
    DataSource a(4);
    DataSource b(4);

    a.init();

    a.setByte(0, 3);
    a.setByte(2, 64);

    REQUIRE(a.getByte(0) == 3);
    REQUIRE(a.getByte(1) == 0);
    REQUIRE(a.getByte(2) == 64);
    REQUIRE(a.getByte(3) == 0);

    b.init(a.getDataPtr());

    REQUIRE(b.getByte(0) == 3);
    REQUIRE(b.getByte(1) == 0);
    REQUIRE(b.getByte(2) == 64);
    REQUIRE(b.getByte(3) == 0);

    b.setByte(0, 0);
    b.setByte(1, 232);

    a.setByte(0, 16);
    a.setByte(3, 128);

    REQUIRE(a.getByte(0) == 16);
    REQUIRE(a.getByte(1) == 0);
    REQUIRE(a.getByte(2) == 64);
    REQUIRE(a.getByte(3) == 128);

    REQUIRE(b.getByte(0) == 0);
    REQUIRE(b.getByte(1) == 232);
    REQUIRE(b.getByte(2) == 64);
    REQUIRE(b.getByte(3) == 0);
}

TEST_CASE("Source Event Clear", "[source]")
{
    DataSource source(4);

    auto flagCallback = [&](bool, bool) {};
    auto byteCallback = [&](uint8_t, uint8_t) {};

    source.init();

    auto &eventMap = EventTestHelper::getUpdateEventListenerMap(source);
    REQUIRE(eventMap.size() == 0);

    auto aToken = source.registerFlagCallback(0, 0x1, flagCallback);
    REQUIRE(eventMap.size() == 1);

    auto bToken = source.registerFlagCallback(0, 0x1, flagCallback);
    REQUIRE(eventMap.size() == 2);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    bToken.unregister();
    REQUIRE(eventMap.size() == 0);

    aToken = source.registerByteCallback(2, byteCallback);
    REQUIRE(eventMap.size() == 1);

    bToken = source.registerByteCallback(3, byteCallback);
    REQUIRE(eventMap.size() == 2);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    bToken.unregister();
    REQUIRE(eventMap.size() == 0);
}