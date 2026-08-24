#include <catch2/catch_test_macros.hpp>
#include <campaign/layout.hpp>

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

    auto dDesc = layout.getDynamicInfo("dynamic_byte", 1);
    REQUIRE(dDesc.getType() == Descriptor::Type::Dynamic);
    REQUIRE(dDesc.index == 0);
    REQUIRE(dDesc.size == 1);

    REQUIRE_THROWS(layout.getDynamicInfo("dynamic_byte", 2));
    REQUIRE_THROWS(layout.getDynamicInfo("dynamic_int", 1));

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 5);

    dDesc = layout.getDynamicInfo("dynamic_int", 4);
    REQUIRE(dDesc.index == 1);
    REQUIRE(dDesc.size == 4);

    REQUIRE_THROWS(layout.getDynamicInfo("dynamic_int", 1));

    layout.pushDynamic("dynamic_char", 1);
    REQUIRE(layout.getDataSize() == 6);

    dDesc = layout.getDynamicInfo("dynamic_char", 1);
    REQUIRE(dDesc.index == 5);
    REQUIRE(dDesc.size == 1);

    REQUIRE_THROWS(layout.getDynamicInfo("dynamic_char", 2));
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

    auto dDesc = layout.getDynamicInfo("dynamic_int", 4);
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
    REQUIRE_THROWS(layout.getDynamicInfo("flag_first", 4));

    REQUIRE_THROWS(layout.getFlagInfo("byte_first"));
    REQUIRE_THROWS(layout.getDynamicInfo("byte_first", 4));

    REQUIRE_THROWS(layout.getFlagInfo("dynamic_int"));
    REQUIRE_THROWS(layout.getByteInfo("dynamic_int"));
}