#include <catch2/catch_test_macros.hpp>
#include <campaign/layout.hpp>
#include <unordered_set>

using namespace campaign;

TEST_CASE("Layout Flag Push", "[layout]")
{
    Layout layout(9);

    REQUIRE(layout.getDataSize() == 0);

    layout.pushFlag("flag_first");

    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag("flag_second");
    layout.pushFlag("flag_third");
    layout.pushFlag("flag_fourth");

    REQUIRE(layout.getDataSize() == 1);

    auto fDesc = layout.getFlagInfo("flag_first");

    REQUIRE(fDesc.getType() == Descriptor::Type::Flag);
    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagInfo("flag_second");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 1));

    fDesc = layout.getFlagInfo("flag_third");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 2));

    fDesc = layout.getFlagInfo("flag_fourth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 3));

    REQUIRE_THROWS(layout.getFlagInfo("flag_fifth"));

    layout.pushFlag("flag_fifth");

    fDesc = layout.getFlagInfo("flag_fifth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 4));

    layout.pushFlag("flag_sixth");
    layout.pushFlag("flag_seventh");
    layout.pushFlag("flag_eighth");

    REQUIRE(layout.getDataSize() == 1);

    fDesc = layout.getFlagInfo("flag_sixth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 5));

    fDesc = layout.getFlagInfo("flag_seventh");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 6));

    fDesc = layout.getFlagInfo("flag_eighth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 7));

    layout.pushFlag("flag_ninth");

    REQUIRE(layout.getDataSize() == 2);

    fDesc = layout.getFlagInfo("flag_ninth");

    REQUIRE(fDesc.index == 1);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagInfo("flag_first");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));
}

TEST_CASE("Layout Flag Push Empty", "[layout]")
{
    Layout layout(10);

    REQUIRE(layout.getDataSize() == 0);

    layout.pushFlag();

    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag();
    layout.pushFlag();
    layout.pushFlag();

    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag();
    layout.pushFlag();
    layout.pushFlag();
    layout.pushFlag();

    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag();

    REQUIRE(layout.getDataSize() == 2);

    layout.pushFlag("flag");

    auto fDesc = layout.getFlagInfo("flag");

    REQUIRE(fDesc.index == 1);
    REQUIRE(fDesc.flagMask == (0x1 << 1));
}

TEST_CASE("Layout Byte Push", "[layout]")
{
    Layout layout(4);

    layout.pushByte("byte_first");

    REQUIRE(layout.getDataSize() == 1);

    auto bDesc = layout.getByteInfo("byte_first");
    REQUIRE(bDesc.getType() == Descriptor::Type::Byte);
    REQUIRE(bDesc.index == 0);

    REQUIRE_THROWS(layout.getByteInfo("byte_second"));

    layout.pushByte("byte_second");
    REQUIRE(layout.getDataSize() == 2);

    bDesc = layout.getByteInfo("byte_second");
    REQUIRE(bDesc.index == 1);

    bDesc = layout.getByteInfo("byte_first");
    REQUIRE(bDesc.index == 0);

    layout.pushByte("byte_third");
    layout.pushByte("byte_fourth");
    REQUIRE(layout.getDataSize() == 4);

    bDesc = layout.getByteInfo("byte_third");
    REQUIRE(bDesc.index == 2);

    bDesc = layout.getByteInfo("byte_fourth");
    REQUIRE(bDesc.index == 3);
}

TEST_CASE("Layout Byte Push Empty", "[layout]")
{
    Layout layout(4);

    layout.pushByte();

    REQUIRE(layout.getDataSize() == 1);

    layout.pushByte();
    REQUIRE(layout.getDataSize() == 2);

    layout.pushByte();
    layout.pushByte();
    REQUIRE(layout.getDataSize() == 4);
}

TEST_CASE("Layout Dynamic Push", "[layout]")
{
    Layout layout(4);

    layout.pushDynamic("dynamic_byte", 1);
    REQUIRE(layout.getDataSize() == 1);

    auto dDesc = layout.getDynamicInfo("dynamic_byte");
    REQUIRE(dDesc.getType() == Descriptor::Type::Dynamic);
    REQUIRE(dDesc.index == 0);
    REQUIRE(dDesc.size == 1);

    REQUIRE_THROWS(layout.getDynamicInfo("dynamic_int"));

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 5);

    dDesc = layout.getDynamicInfo("dynamic_int");
    REQUIRE(dDesc.index == 1);
    REQUIRE(dDesc.size == 4);

    layout.pushDynamic("dynamic_char", 1);
    REQUIRE(layout.getDataSize() == 6);

    dDesc = layout.getDynamicInfo("dynamic_char");
    REQUIRE(dDesc.index == 5);
    REQUIRE(dDesc.size == 1);
}

TEST_CASE("Layout Dynamic Push Empty", "[layout]")
{
    Layout layout(4);

    layout.pushDynamic(1);
    REQUIRE(layout.getDataSize() == 1);

    layout.pushDynamic(4);
    REQUIRE(layout.getDataSize() == 5);

    layout.pushDynamic(1);
    REQUIRE(layout.getDataSize() == 6);
}

TEST_CASE("Layout Mixed Push", "[layout]")
{
    Layout layout(6);

    layout.pushFlag("flag_first");
    REQUIRE(layout.getDataSize() == 1);

    auto fDesc = layout.getFlagInfo("flag_first");
    REQUIRE(fDesc.getType() == Descriptor::Type::Flag);
    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    layout.pushByte("byte_first");
    REQUIRE(layout.getDataSize() == 2);

    auto bDesc = layout.getByteInfo("byte_first");
    REQUIRE(bDesc.getType() == Descriptor::Type::Byte);
    REQUIRE(bDesc.index == 1);

    layout.pushFlag("flag_second");
    layout.pushFlag("flag_third");
    REQUIRE(layout.getDataSize() == 3);

    fDesc = layout.getFlagInfo("flag_second");
    REQUIRE(fDesc.getType() == Descriptor::Type::Flag);
    REQUIRE(fDesc.index == 2);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagInfo("flag_third");
    REQUIRE(fDesc.getType() == Descriptor::Type::Flag);
    REQUIRE(fDesc.index == 2);
    REQUIRE(fDesc.flagMask == (0x1 << 1));

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 7);

    auto dDesc = layout.getDynamicInfo("dynamic_int");
    REQUIRE(dDesc.getType() == Descriptor::Type::Dynamic);
    REQUIRE(dDesc.index == 3);
    REQUIRE(dDesc.size == 4);

    layout.pushFlag("flag_fourth");
    REQUIRE(layout.getDataSize() == 8);

    fDesc = layout.getFlagInfo("flag_fourth");
    REQUIRE(fDesc.getType() == Descriptor::Type::Flag);
    REQUIRE(fDesc.index == 7);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    REQUIRE_THROWS(layout.getByteInfo("flag_first"));
    REQUIRE_THROWS(layout.getDynamicInfo("flag_first"));

    REQUIRE_THROWS(layout.getFlagInfo("byte_first"));
    REQUIRE_THROWS(layout.getDynamicInfo("byte_first"));

    REQUIRE_THROWS(layout.getFlagInfo("dynamic_int"));
    REQUIRE_THROWS(layout.getByteInfo("dynamic_int"));
}

TEST_CASE ("Layout Entry Exists", "[layout]")
{
    Layout layout(3);

    REQUIRE_FALSE(layout.entryExists("flag_first"));
    layout.pushFlag("flag_first");
    REQUIRE(layout.entryExists("flag_first"));

    REQUIRE_FALSE(layout.entryExists("byte_first"));
    layout.pushByte("byte_first");
    REQUIRE(layout.entryExists("byte_first"));

    REQUIRE_FALSE(layout.entryExists("dynamic_int"));
    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.entryExists("dynamic_int"));
}

TEST_CASE ("Layout Get Entry Count", "[layout]")
{
    Layout layout(3);

    REQUIRE(layout.getEntryCount() == 0);

    layout.pushFlag("flag_first");
    REQUIRE(layout.getEntryCount() == 1);

    layout.pushByte("byte_first");
    REQUIRE(layout.getEntryCount() == 2);

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getEntryCount() == 3);

    layout.pushFlag();
    REQUIRE(layout.getEntryCount() == 3);

    layout.pushByte();
    REQUIRE(layout.getEntryCount() == 3);

    layout.pushDynamic(4);
    REQUIRE(layout.getEntryCount() == 3);
}

TEST_CASE ("Layout Get Data Size", "[layout]")
{
    Layout layout(5);

    REQUIRE(layout.getDataSize() == 0);

    layout.pushFlag("flag_first");
    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag("flag_second");
    REQUIRE(layout.getDataSize() == 1);

    layout.pushByte("byte_first");
    REQUIRE(layout.getDataSize() == 2);

    layout.pushFlag("flag_third");
    REQUIRE(layout.getDataSize() == 3);

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 7);

    layout.pushFlag();
    REQUIRE(layout.getDataSize() == 8);

    layout.pushByte();
    REQUIRE(layout.getDataSize() == 9);

    layout.pushDynamic(4);
    REQUIRE(layout.getDataSize() == 13);
}

TEST_CASE("Layout Get Entry Info", "[layout]")
{
    Layout layout(5);

    REQUIRE_THROWS(layout.getEntryInfo(0));

    layout.pushFlag("flag_first");
    layout.pushFlag("flag_second");
    layout.pushByte("byte_first");
    layout.pushFlag("flag_third");
    layout.pushDynamic("dynamic_int", 4);

    std::unordered_set<std::string> requiredIds = {"flag_first", "flag_second", "byte_first", "flag_third", "dynamic_int"};

    for (int i = 0; i < layout.getEntryCount(); i++)
    {
        auto entryInfo = layout.getEntryInfo(i);

        if (entryInfo.id == "flag_first")
        {
            REQUIRE(entryInfo.descriptor.getType() == Descriptor::Type::Flag);

            auto flagInfo = entryInfo.descriptor.getFlagInfo();
            REQUIRE(flagInfo.index == 0);
            REQUIRE(flagInfo.flagMask == (0x1 << 0));
            continue;
        }

        if (entryInfo.id == "flag_second")
        {
            REQUIRE(entryInfo.descriptor.getType() == Descriptor::Type::Flag);

            auto flagInfo = entryInfo.descriptor.getFlagInfo();
            REQUIRE(flagInfo.index == 0);
            REQUIRE(flagInfo.flagMask == (0x1 << 1));
            continue;
        }

        if (entryInfo.id == "byte_first")
        {
            REQUIRE(entryInfo.descriptor.getType() == Descriptor::Type::Byte);

            auto byteInfo = entryInfo.descriptor.getByteInfo();
            REQUIRE(byteInfo.index == 1);
            continue;
        }

        if (entryInfo.id == "flag_third")
        {
            REQUIRE(entryInfo.descriptor.getType() == Descriptor::Type::Flag);

            auto flagInfo = entryInfo.descriptor.getFlagInfo();
            REQUIRE(flagInfo.index == 2);
            REQUIRE(flagInfo.flagMask == (0x1 << 0));
            continue;
        }

        if (entryInfo.id == "dynamic_int")
        {
            REQUIRE(entryInfo.descriptor.getType() == Descriptor::Type::Dynamic);

            auto dynamicInfo = entryInfo.descriptor.getDynamicInfo();
            REQUIRE(dynamicInfo.index == 3);
            REQUIRE(dynamicInfo.size == 4);
            continue;
        }

        FAIL("Didn't expact an entry of id \"" + entryInfo.id + "\".");
    }

    REQUIRE_THROWS(layout.getEntryInfo(5));
}

TEST_CASE("Layout Get Flag Info", "[layout]")
{
    Layout layout(5);

    REQUIRE_THROWS(layout.getFlagInfo("flag_first"));
    REQUIRE_THROWS(layout.getFlagInfo("flag_second"));

    layout.pushFlag("flag_first");
    
    REQUIRE_THROWS(layout.getFlagInfo("flag_second"));

    layout.pushFlag("flag_second");

    auto flagInfo = layout.getFlagInfo("flag_first");

    REQUIRE(flagInfo.index == 0);
    REQUIRE(flagInfo.flagMask == (0x1 << 0));

    flagInfo = layout.getFlagInfo("flag_second");

    REQUIRE(flagInfo.index == 0);
    REQUIRE(flagInfo.flagMask == (0x1 << 1));

    layout.pushByte();
    layout.pushFlag("flag_third");

    flagInfo = layout.getFlagInfo("flag_third");

    REQUIRE(flagInfo.index == 2);
    REQUIRE(flagInfo.flagMask == (0x1 << 0));

    layout.pushByte("byte");
    layout.pushDynamic("dynamic", 4);

    REQUIRE_THROWS(layout.getFlagInfo("byte"));
    REQUIRE_THROWS(layout.getFlagInfo("dynamic"));
}

TEST_CASE("Layout Get Byte Info", "[layout]")
{
    Layout layout(4);

    REQUIRE_THROWS(layout.getByteInfo("byte_first"));
    REQUIRE_THROWS(layout.getByteInfo("byte_second"));

    layout.pushByte("byte_first");
    
    REQUIRE_THROWS(layout.getByteInfo("byte_second"));

    layout.pushByte("byte_second");

    auto byteInfo = layout.getByteInfo("byte_first");

    REQUIRE(byteInfo.index == 0);

    byteInfo = layout.getByteInfo("byte_second");

    REQUIRE(byteInfo.index == 1);

    layout.pushFlag("flag");
    layout.pushDynamic("dynamic", 4);

    REQUIRE_THROWS(layout.getByteInfo("flag"));
    REQUIRE_THROWS(layout.getByteInfo("dynamic"));
}

TEST_CASE("Layout Get Dynamic Info", "[layout]")
{
    Layout layout(4);

    REQUIRE_THROWS(layout.getByteInfo("dynamic_int"));
    REQUIRE_THROWS(layout.getByteInfo("dynamic_double"));

    layout.pushDynamic("dynamic_int", 4);
    
    REQUIRE_THROWS(layout.getByteInfo("dynamic_double"));

    layout.pushDynamic("dynamic_double", 8);

    auto dynamicInfo = layout.getDynamicInfo("dynamic_int");

    REQUIRE(dynamicInfo.index == 0);
    REQUIRE(dynamicInfo.size == 4);

    dynamicInfo = layout.getDynamicInfo("dynamic_double");

    REQUIRE(dynamicInfo.index == 4);
    REQUIRE(dynamicInfo.size == 8);

    layout.pushFlag("flag");
    layout.pushByte("byte");

    REQUIRE_THROWS(layout.getDynamicInfo("flag"));
    REQUIRE_THROWS(layout.getDynamicInfo("byte"));
}