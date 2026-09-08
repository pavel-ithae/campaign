#include <catch2/catch_test_macros.hpp>
#include <campaign/blueprint.hpp>

#include <unordered_set>

using namespace campaign;

TEST_CASE("Blueprint", "[blueprint]")
{
    Blueprint blueprint(4);

    REQUIRE(blueprint.size() == 0);

    Layout layout_a(3);
    layout_a.pushFlag("flag");
    layout_a.pushByte("byte");
    layout_a.pushDynamic("dynamic", 4);

    Layout layout_b(2);
    layout_b.pushByte("b_00");
    layout_b.pushByte("b_01");

    blueprint.push(BlueprintPiece("layout_a", layout_a));
    REQUIRE(blueprint.size() == 1);

    blueprint.push(BlueprintPiece("layout_b", layout_b));
    REQUIRE(blueprint.size() == 2);

    std::unordered_set<std::string> requiredIds = {"layout_a", "layout_b"};

    for (auto bPiece : blueprint)
    {
        if (bPiece.id == "layout_a")
        {
            REQUIRE(bPiece.layout.entryExists("flag", Descriptor::Type::Flag));
            REQUIRE(bPiece.layout.entryExists("byte", Descriptor::Type::Byte));
            REQUIRE(bPiece.layout.entryExists("dynamic", Descriptor::Type::Dynamic));

            REQUIRE_FALSE(bPiece.layout.entryExists("b_00", Descriptor::Type::Byte));
            REQUIRE_FALSE(bPiece.layout.entryExists("b_01", Descriptor::Type::Byte));

            requiredIds.erase("layout_a");
            continue;
        }

        if (bPiece.id == "layout_b")
        {
            REQUIRE(bPiece.layout.entryExists("b_00", Descriptor::Type::Byte));
            REQUIRE(bPiece.layout.entryExists("b_01", Descriptor::Type::Byte));

            REQUIRE_FALSE(bPiece.layout.entryExists("flag", Descriptor::Type::Flag));
            REQUIRE_FALSE(bPiece.layout.entryExists("byte", Descriptor::Type::Byte));
            REQUIRE_FALSE(bPiece.layout.entryExists("dynamic", Descriptor::Type::Dynamic));

            requiredIds.erase("layout_b");
            continue;
        }

        FAIL("Didn't expact an entry of id \"" + bPiece.id + "\".");
    }

    REQUIRE(requiredIds.size() == 0);
}