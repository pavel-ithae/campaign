#include <catch2/catch_test_macros.hpp>
#include <campaign/data_layout.hpp>

using namespace campaign;

TEST_CASE("Layout Flag Push", "[layout]")
{
    DataLayout layout(9);

    REQUIRE(layout.getDataSize() == 0);

    layout.pushFlag("flag_first");

    REQUIRE(layout.getDataSize() == 1);

    layout.pushFlag("flag_second");
    layout.pushFlag("flag_third");
    layout.pushFlag("flag_fourth");

    REQUIRE(layout.getDataSize() == 1);

    auto fDesc = layout.getFlagDescription("flag_first");

    REQUIRE(fDesc.getType() == DataDescriptor::Type::Flag);
    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagDescription("flag_second");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 1));

    fDesc = layout.getFlagDescription("flag_third");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 2));

    fDesc = layout.getFlagDescription("flag_fourth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 3));

    REQUIRE_THROWS(layout.getFlagDescription("flag_fifth"));

    layout.pushFlag("flag_fifth");

    fDesc = layout.getFlagDescription("flag_fifth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 4));

    layout.pushFlag("flag_sixth");
    layout.pushFlag("flag_seventh");
    layout.pushFlag("flag_eighth");

    REQUIRE(layout.getDataSize() == 1);

    fDesc = layout.getFlagDescription("flag_sixth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 5));

    fDesc = layout.getFlagDescription("flag_seventh");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 6));

    fDesc = layout.getFlagDescription("flag_eighth");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 7));

    layout.pushFlag("flag_ninth");

    REQUIRE(layout.getDataSize() == 2);

    fDesc = layout.getFlagDescription("flag_ninth");

    REQUIRE(fDesc.index == 1);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagDescription("flag_first");

    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));
}

TEST_CASE("Layout Flag Push Empty", "[layout]")
{
    DataLayout layout(10);

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

    auto fDesc = layout.getFlagDescription("flag");

    REQUIRE(fDesc.index == 1);
    REQUIRE(fDesc.flagMask == (0x1 << 1));
}

TEST_CASE("Layout Byte Push", "[layout]")
{
    DataLayout layout(4);

    layout.pushByte("byte_first");

    REQUIRE(layout.getDataSize() == 1);

    auto bDesc = layout.getByteDescription("byte_first");
    REQUIRE(bDesc.getType() == DataDescriptor::Type::Byte);
    REQUIRE(bDesc.index == 0);

    REQUIRE_THROWS(layout.getByteDescription("byte_second"));

    layout.pushByte("byte_second");
    REQUIRE(layout.getDataSize() == 2);

    bDesc = layout.getByteDescription("byte_second");
    REQUIRE(bDesc.index == 1);

    bDesc = layout.getByteDescription("byte_first");
    REQUIRE(bDesc.index == 0);

    layout.pushByte("byte_third");
    layout.pushByte("byte_fourth");
    REQUIRE(layout.getDataSize() == 4);

    bDesc = layout.getByteDescription("byte_third");
    REQUIRE(bDesc.index == 2);

    bDesc = layout.getByteDescription("byte_fourth");
    REQUIRE(bDesc.index == 3);
}

TEST_CASE("Layout Byte Push Empty", "[layout]")
{
    DataLayout layout(4);

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
    DataLayout layout(4);

    layout.pushDynamic("dynamic_byte", 1);
    REQUIRE(layout.getDataSize() == 1);

    auto dDesc = layout.getDynamicDescription("dynamic_byte", 1);
    REQUIRE(dDesc.getType() == DataDescriptor::Type::Dynamic);
    REQUIRE(dDesc.index == 0);
    REQUIRE(dDesc.size == 1);

    REQUIRE_THROWS(layout.getDynamicDescription("dynamic_byte", 2));
    REQUIRE_THROWS(layout.getDynamicDescription("dynamic_int", 1));

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 5);

    dDesc = layout.getDynamicDescription("dynamic_int", 4);
    REQUIRE(dDesc.index == 1);
    REQUIRE(dDesc.size == 4);

    REQUIRE_THROWS(layout.getDynamicDescription("dynamic_int", 1));

    layout.pushDynamic("dynamic_char", 1);
    REQUIRE(layout.getDataSize() == 6);

    dDesc = layout.getDynamicDescription("dynamic_char", 1);
    REQUIRE(dDesc.index == 5);
    REQUIRE(dDesc.size == 1);

    REQUIRE_THROWS(layout.getDynamicDescription("dynamic_char", 2));
}

TEST_CASE("Layout Dynamic Push Empty", "[layout]")
{
    DataLayout layout(4);

    layout.pushDynamic(1);
    REQUIRE(layout.getDataSize() == 1);

    layout.pushDynamic(4);
    REQUIRE(layout.getDataSize() == 5);

    layout.pushDynamic(1);
    REQUIRE(layout.getDataSize() == 6);
}

TEST_CASE("Layout Mixed Push", "[layout]")
{
    DataLayout layout(6);

    layout.pushFlag("flag_first");
    REQUIRE(layout.getDataSize() == 1);

    auto fDesc = layout.getFlagDescription("flag_first");
    REQUIRE(fDesc.getType() == DataDescriptor::Type::Flag);
    REQUIRE(fDesc.index == 0);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    layout.pushByte("byte_first");
    REQUIRE(layout.getDataSize() == 2);

    auto bDesc = layout.getByteDescription("byte_first");
    REQUIRE(bDesc.getType() == DataDescriptor::Type::Byte);
    REQUIRE(bDesc.index == 1);

    layout.pushFlag("flag_second");
    layout.pushFlag("flag_third");
    REQUIRE(layout.getDataSize() == 3);

    fDesc = layout.getFlagDescription("flag_second");
    REQUIRE(fDesc.getType() == DataDescriptor::Type::Flag);
    REQUIRE(fDesc.index == 2);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    fDesc = layout.getFlagDescription("flag_third");
    REQUIRE(fDesc.getType() == DataDescriptor::Type::Flag);
    REQUIRE(fDesc.index == 2);
    REQUIRE(fDesc.flagMask == (0x1 << 1));

    layout.pushDynamic("dynamic_int", 4);
    REQUIRE(layout.getDataSize() == 7);

    auto dDesc = layout.getDynamicDescription("dynamic_int", 4);
    REQUIRE(dDesc.getType() == DataDescriptor::Type::Dynamic);
    REQUIRE(dDesc.index == 3);
    REQUIRE(dDesc.size == 4);

    layout.pushFlag("flag_fourth");
    REQUIRE(layout.getDataSize() == 8);

    fDesc = layout.getFlagDescription("flag_fourth");
    REQUIRE(fDesc.getType() == DataDescriptor::Type::Flag);
    REQUIRE(fDesc.index == 7);
    REQUIRE(fDesc.flagMask == (0x1 << 0));

    REQUIRE_THROWS(layout.getByteDescription("flag_first"));
    REQUIRE_THROWS(layout.getDynamicDescription("flag_first", 4));

    REQUIRE_THROWS(layout.getFlagDescription("byte_first"));
    REQUIRE_THROWS(layout.getDynamicDescription("byte_first", 4));

    REQUIRE_THROWS(layout.getFlagDescription("dynamic_int"));
    REQUIRE_THROWS(layout.getByteDescription("dynamic_int"));
}