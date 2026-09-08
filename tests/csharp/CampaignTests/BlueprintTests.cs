namespace Campaign.Tests;

public static class BlueprintTests
{
    [Fact]
    public static void Blueprint()
    {
        var blueprint = new Blueprint(4);

        Assert.Equal(0, blueprint.count);

        var layout_a = new Layout(3);
        layout_a.PushFlag("flag");
        layout_a.PushByte("byte");
        layout_a.PushDynamic("dynamic", 4);

        var layout_b = new Layout(2);
        layout_b.PushByte("b_00");
        layout_b.PushByte("b_01");

        blueprint.Push("layout_a", layout_a);
        Assert.Equal(1, blueprint.count);

        blueprint.Push("layout_b", layout_b);
        Assert.Equal(2, blueprint.count);

        HashSet<string> requiredIds = new HashSet<string>() { "layout_a", "layout_b" };

        foreach (var piece in blueprint)
        {
            switch (piece.id)
            {
                case "layout_a":
                    Assert.True(piece.layout?.EntryExist("flag", Descriptor.InfoType.Flag));
                    Assert.True(piece.layout?.EntryExist("byte", Descriptor.InfoType.Byte));
                    Assert.True(piece.layout?.EntryExist("dynamic", Descriptor.InfoType.Dynamic));

                    Assert.False(piece.layout?.EntryExist("b_00", Descriptor.InfoType.Byte));
                    Assert.False(piece.layout?.EntryExist("b_01", Descriptor.InfoType.Byte));

                    requiredIds.Remove("layout_a");
                    break;

                case "layout_b":
                    Assert.True(piece.layout?.EntryExist("b_00", Descriptor.InfoType.Byte));
                    Assert.True(piece.layout?.EntryExist("b_01", Descriptor.InfoType.Byte));

                    Assert.False(piece.layout?.EntryExist("flag", Descriptor.InfoType.Flag));
                    Assert.False(piece.layout?.EntryExist("byte", Descriptor.InfoType.Byte));
                    Assert.False(piece.layout?.EntryExist("dynamic", Descriptor.InfoType.Dynamic));

                    requiredIds.Remove("layout_b");
                    break;

                default:
                    Assert.Fail($"Didn't expect an entry of id \"{piece.id}\".");
                    break;
            }
        }

        Assert.Empty(requiredIds);
    }
}