#include <catch2/catch_test_macros.hpp>
#include <campaign/proxy.hpp>
#include <campaign/testing/event_test_helper.hpp>
#include <array>

using namespace campaign;
using namespace campaign::testing;

#define INIT_SOURCE_DATA(name, size) std::array<uint8_t, size> name; \
    std::fill(name.begin(), name.end(), 0)

#define RESET_SOURCE_DATA(name) std::fill(name.begin(), name.end(), 0)

TEST_CASE("Proxy Init", "[proxy]")
{
    INIT_SOURCE_DATA(data4, 4);
    Proxy proxy(data4.begin(), data4.end());

    REQUIRE(proxy.getSize() == 4);

    REQUIRE(proxy.getByte(0) == 0x0);
    REQUIRE(proxy.getByte(1) == 0x0);
    REQUIRE(proxy.getByte(2) == 0x0);
    REQUIRE(proxy.getByte(3) == 0x0);

    INIT_SOURCE_DATA(data8, 8);
    proxy = Proxy(data8.begin(), data8.end());

    REQUIRE(proxy.getSize() == 8);

    REQUIRE(proxy.getByte(0) == 0x0);
    REQUIRE(proxy.getByte(1) == 0x0);
    REQUIRE(proxy.getByte(2) == 0x0);
    REQUIRE(proxy.getByte(3) == 0x0);
    REQUIRE(proxy.getByte(4) == 0x0);
    REQUIRE(proxy.getByte(5) == 0x0);
    REQUIRE(proxy.getByte(6) == 0x0);
    REQUIRE(proxy.getByte(7) == 0x0);
}

TEST_CASE("Proxy Flags", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    REQUIRE(proxy.getFlag(0, 0x1 << 0) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 1) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 2) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 3) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 0) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 1) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 2) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 3) == false);

    proxy.setFlag(0, 0x1 << 0, true);
    proxy.setFlag(0, 0x1 << 3, true);

    REQUIRE(proxy.getFlag(0, 0x1 << 0) == true);
    REQUIRE(proxy.getFlag(0, 0x1 << 1) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 2) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 3) == true);
    REQUIRE(proxy.getFlag(1, 0x1 << 0) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 1) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 2) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 3) == false);

    proxy.setFlag(1, 0x1 << 1, true);
    proxy.setFlag(1, 0x1 << 2, true);

    REQUIRE(proxy.getFlag(0, 0x1 << 0) == true);
    REQUIRE(proxy.getFlag(0, 0x1 << 1) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 2) == false);
    REQUIRE(proxy.getFlag(0, 0x1 << 3) == true);
    REQUIRE(proxy.getFlag(1, 0x1 << 0) == false);
    REQUIRE(proxy.getFlag(1, 0x1 << 1) == true);
    REQUIRE(proxy.getFlag(1, 0x1 << 2) == true);
    REQUIRE(proxy.getFlag(1, 0x1 << 3) == false);
}

TEST_CASE("Proxy Bytes", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    proxy.setByte(0, 12);
    proxy.setByte(3, 32);

    REQUIRE(proxy.getByte(0) == 12);
    REQUIRE(proxy.getByte(1) == 0);
    REQUIRE(proxy.getByte(2) == 0);
    REQUIRE(proxy.getByte(3) == 32);

    proxy.setByte(1, 64);
    proxy.setByte(2, 255);

    REQUIRE(proxy.getByte(0) == 12);
    REQUIRE(proxy.getByte(1) == 64);
    REQUIRE(proxy.getByte(2) == 255);
    REQUIRE(proxy.getByte(3) == 32);
}

TEST_CASE("Proxy Dynamic", "[proxy]")
{
    INIT_SOURCE_DATA(data, 8);
    Proxy proxy(data.begin(), data.end());

    proxy.set<uint32_t>(0, 255);

    REQUIRE(proxy.get<uint32_t>(0) == 255);
    REQUIRE(proxy.getByte(0) == 255);
    REQUIRE(proxy.getByte(1) == 0);
    REQUIRE(proxy.getByte(2) == 0);
    REQUIRE(proxy.getByte(3) == 0);
    REQUIRE(proxy.getByte(4) == 0);
    REQUIRE(proxy.getByte(5) == 0);
    REQUIRE(proxy.getByte(6) == 0);
    REQUIRE(proxy.getByte(7) == 0);

    proxy.set<uint32_t>(0, 256);
    REQUIRE(proxy.get<uint32_t>(0) == 256);
    REQUIRE(proxy.getByte(0) == 0);
    REQUIRE(proxy.getByte(1) == 1);
    REQUIRE(proxy.getByte(2) == 0);
    REQUIRE(proxy.getByte(3) == 0);
    REQUIRE(proxy.getByte(4) == 0);
    REQUIRE(proxy.getByte(5) == 0);
    REQUIRE(proxy.getByte(6) == 0);
    REQUIRE(proxy.getByte(7) == 0);

    proxy.set<uint32_t>(3, 255);
    REQUIRE(proxy.get<uint32_t>(3) == 255);
    REQUIRE(proxy.getByte(0) == 0);
    REQUIRE(proxy.getByte(1) == 1);
    REQUIRE(proxy.getByte(2) == 0);
    REQUIRE(proxy.getByte(3) == 255);
    REQUIRE(proxy.getByte(4) == 0);
    REQUIRE(proxy.getByte(5) == 0);
    REQUIRE(proxy.getByte(6) == 0);
    REQUIRE(proxy.getByte(7) == 0);
}

#if CAMPAIGN_SOURCE_CHECK_BOUNDS
TEST_CASE("Proxy Bounds Checking", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    CHECK_NOTHROW([&]()
                 { return proxy.getFlag(3, 0x1); }());
    CHECK_NOTHROW([&]()
                 { return proxy.getByte(3) == 0x0; }());
    CHECK_NOTHROW([&]()
                 { return proxy.get<uint32_t>(0); }());

    CHECK_THROWS([&]()
                 { return proxy.getFlag(4, 0x1); }());
    CHECK_THROWS([&]()
                 { return proxy.getByte(4) == 0x0; }());
    CHECK_THROWS([&]()
                 { return proxy.get<uint32_t>(1); }());
    CHECK_THROWS([&]()
                 { return proxy.get<uint64_t>(0); }());
}
#endif

TEST_CASE("Proxy Flag Notification", "[proxy]")
{
    int callCount = 0;
    auto callback = [&](bool previous, bool current)
    {
        callCount++;
    };

    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    auto token = proxy.registerFlagCallback(0, 0x1 << 0, callback);

    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 1);
    proxy.setFlag(0, 0x1 << 1, true);
    REQUIRE(callCount == 1);
    proxy.setFlag(0, 0x1 << 1, false);
    REQUIRE(callCount == 1);
    proxy.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 2);
    proxy.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 2);
    proxy.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 0);

    callCount = 0;
    RESET_SOURCE_DATA(data);

    token = proxy.registerFlagCallback(2, 0x1 << 4, callback);

    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 0, false);
    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 4, true);
    REQUIRE(callCount == 0);
    proxy.setFlag(0, 0x1 << 4, false);
    REQUIRE(callCount == 0);
    proxy.setFlag(2, 0x1 << 0, true);
    REQUIRE(callCount == 0);
    proxy.setFlag(2, 0x1 << 0, false);
    REQUIRE(callCount == 0);
    proxy.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 1);
    proxy.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 2);
    proxy.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 2);
    proxy.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 3);
    proxy.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 3);

    token.unregister();

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setFlag(2, 0x1 << 4, true);
    REQUIRE(callCount == 0);
    proxy.setFlag(2, 0x1 << 4, false);
    REQUIRE(callCount == 0);
}

TEST_CASE("Proxy Byte Notification", "[proxy]")
{
    int callCount = 0;
    auto callback = [&](uint8_t previous, uint8_t current)
    {
        callCount++;
    };

    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    auto token = proxy.registerByteCallback(0, callback);

    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 1);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 1);
    proxy.setByte(0, 25);
    REQUIRE(callCount == 2);
    proxy.setByte(0, 0);
    REQUIRE(callCount == 3);
    proxy.setByte(1, 1);
    REQUIRE(callCount == 3);
    proxy.setByte(1, 36);
    REQUIRE(callCount == 3);
    proxy.setByte(0, 36);
    REQUIRE(callCount == 4);

    token.unregister();

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 25);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 0);
    REQUIRE(callCount == 0);
    proxy.setByte(1, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(1, 36);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 36);
    REQUIRE(callCount == 0);

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 25);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 0);
    REQUIRE(callCount == 0);

    token = proxy.registerByteCallback(3, callback);

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(0, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(1, 25);
    REQUIRE(callCount == 0);
    proxy.setByte(1, 0);
    REQUIRE(callCount == 0);
    proxy.setByte(3, 1);
    REQUIRE(callCount == 1);
    proxy.setByte(3, 36);
    REQUIRE(callCount == 2);
    proxy.setByte(0, 36);
    REQUIRE(callCount == 2);

    token.unregister();

    callCount = 0;
    RESET_SOURCE_DATA(data);

    REQUIRE(callCount == 0);
    proxy.setByte(3, 1);
    REQUIRE(callCount == 0);
    proxy.setByte(3, 25);
    REQUIRE(callCount == 0);
    proxy.setByte(3, 0);
    REQUIRE(callCount == 0);
}

TEST_CASE("Proxy Event Clear", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    Proxy proxy(data.begin(), data.end());

    auto flagCallback = [&](bool, bool) {};
    auto byteCallback = [&](uint8_t, uint8_t) {};

    auto &eventMap = EventTestHelper::getUpdateEventListenerMap(proxy);
    REQUIRE(eventMap.size() == 0);

    auto aToken = proxy.registerFlagCallback(0, 0x1, flagCallback);
    REQUIRE(eventMap.size() == 1);

    auto bToken = proxy.registerFlagCallback(0, 0x1, flagCallback);
    REQUIRE(eventMap.size() == 2);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    bToken.unregister();
    REQUIRE(eventMap.size() == 0);

    aToken = proxy.registerByteCallback(2, byteCallback);
    REQUIRE(eventMap.size() == 1);

    bToken = proxy.registerByteCallback(3, byteCallback);
    REQUIRE(eventMap.size() == 2);

    aToken.unregister();
    REQUIRE(eventMap.size() == 1);

    bToken.unregister();
    REQUIRE(eventMap.size() == 0);
}

TEST_CASE("Proxy Flag Tokens", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    auto proxyPtr = std::make_shared<Proxy>(data.begin(), data.end());

    auto aToken = ProxyFlagToken(proxyPtr, 0, 0x1 << 0);
    auto bToken = ProxyFlagToken(proxyPtr, 0, 0x1 << 3);

    REQUIRE(aToken.get() == false);
    REQUIRE(bToken.get() == false);

    aToken.set(true);

    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == false);

    bToken.set(true);

    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == true);

    REQUIRE(proxyPtr->getFlag(0, 0x1 << 0) == true);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 1) == false);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 2) == false);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 3) == true);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 0) == false);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 1) == false);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 2) == false);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 3) == false);

    aToken = ProxyFlagToken(proxyPtr, 1, 0x1 << 1);
    bToken = ProxyFlagToken(proxyPtr, 1, 0x1 << 2);

    REQUIRE(aToken.get() == false);
    REQUIRE(bToken.get() == false);

    bToken.set(true);

    REQUIRE(aToken.get() == false);
    REQUIRE(bToken.get() == true);

    aToken.set(true);

    REQUIRE(aToken.get() == true);
    REQUIRE(bToken.get() == true);

    REQUIRE(proxyPtr->getFlag(0, 0x1 << 0) == true);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 1) == false);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 2) == false);
    REQUIRE(proxyPtr->getFlag(0, 0x1 << 3) == true);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 0) == false);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 1) == true);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 2) == true);
    REQUIRE(proxyPtr->getFlag(1, 0x1 << 3) == false);
}

TEST_CASE("Proxy Byte Tokens", "[proxy]")
{
    INIT_SOURCE_DATA(data, 4);
    auto proxyPtr = std::make_shared<Proxy>(data.begin(), data.end());

    ProxyByteToken aToken(proxyPtr, 0);
    ProxyByteToken bToken(proxyPtr, 3);

    REQUIRE(aToken.get() == 0);
    REQUIRE(bToken.get() == 0);

    aToken.set(12);

    REQUIRE(aToken.get() == 12);
    REQUIRE(bToken.get() == 0);

    bToken.set(32);

    REQUIRE(aToken.get() == 12);
    REQUIRE(bToken.get() == 32);

    REQUIRE(proxyPtr->getByte(0) == 12);
    REQUIRE(proxyPtr->getByte(1) == 0);
    REQUIRE(proxyPtr->getByte(2) == 0);
    REQUIRE(proxyPtr->getByte(3) == 32);

    aToken = ProxyByteToken(proxyPtr, 1);
    bToken = ProxyByteToken(proxyPtr, 2);

    REQUIRE(aToken.get() == 0);
    REQUIRE(bToken.get() == 0);

    bToken.set(255);

    REQUIRE(aToken.get() == 0);
    REQUIRE(bToken.get() == 255);

    aToken.set(64);

    REQUIRE(aToken.get() == 64);
    REQUIRE(bToken.get() == 255);

    REQUIRE(proxyPtr->getByte(0) == 12);
    REQUIRE(proxyPtr->getByte(1) == 64);
    REQUIRE(proxyPtr->getByte(2) == 255);
    REQUIRE(proxyPtr->getByte(3) == 32);
}

TEST_CASE("Proxy Dynamic Tokens", "[proxy]")
{
    INIT_SOURCE_DATA(data, 8);
    auto proxyPtr = std::make_shared<Proxy>(data.begin(), data.end());

    ProxyDynamicToken<uint32_t> token(proxyPtr, 0);
    REQUIRE(token.get() == 0);

    token.set(255);
    REQUIRE(token.get() == 255);

    REQUIRE(proxyPtr->getByte(0) == 255);
    REQUIRE(proxyPtr->getByte(1) == 0);
    REQUIRE(proxyPtr->getByte(2) == 0);
    REQUIRE(proxyPtr->getByte(3) == 0);
    REQUIRE(proxyPtr->getByte(4) == 0);
    REQUIRE(proxyPtr->getByte(5) == 0);
    REQUIRE(proxyPtr->getByte(6) == 0);
    REQUIRE(proxyPtr->getByte(7) == 0);

    token.set(256);
    REQUIRE(token.get() == 256);

    REQUIRE(proxyPtr->getByte(0) == 0);
    REQUIRE(proxyPtr->getByte(1) == 1);
    REQUIRE(proxyPtr->getByte(2) == 0);
    REQUIRE(proxyPtr->getByte(3) == 0);
    REQUIRE(proxyPtr->getByte(4) == 0);
    REQUIRE(proxyPtr->getByte(5) == 0);
    REQUIRE(proxyPtr->getByte(6) == 0);
    REQUIRE(proxyPtr->getByte(7) == 0);

    token = ProxyDynamicToken<uint32_t>(proxyPtr, 3);
    REQUIRE(token.get() == 0);

    token.set(255);
    REQUIRE(token.get() == 255);

    REQUIRE(proxyPtr->getByte(0) == 0);
    REQUIRE(proxyPtr->getByte(1) == 1);
    REQUIRE(proxyPtr->getByte(2) == 0);
    REQUIRE(proxyPtr->getByte(3) == 255);
    REQUIRE(proxyPtr->getByte(4) == 0);
    REQUIRE(proxyPtr->getByte(5) == 0);
    REQUIRE(proxyPtr->getByte(6) == 0);
    REQUIRE(proxyPtr->getByte(7) == 0);
}