namespace Campaign.Tests;

public static class LayoutTests
{
    [Fact]
    public static void CreateDelete()
    {
        var layout = new Layout(4);

        layout.PushFlag();

        ((IDisposable)layout).Dispose();

        Assert.Throws<CampaignException>(() => layout.PushFlag());
    }

    [Fact]
    public static void FlagPush()
    {
        var layout = new Layout(9);

        Assert.Equal(0, layout.GetDataSize());

        layout.PushFlag("flag_first");

        Assert.Equal(1, layout.GetDataSize());

        layout.PushFlag("flag_second");
        layout.PushFlag("flag_third");
        layout.PushFlag("flag_fourth");

        Assert.Equal(1, layout.GetDataSize());

        var flagInfo = layout.GetFlagInfo("flag_first");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal(0x1 << 0, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_second");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_third");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 2, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_fourth");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 3, flagInfo.flagMask);

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("flag_fifth"));

        layout.PushFlag("flag_fifth");

        flagInfo = layout.GetFlagInfo("flag_fifth");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 4, flagInfo.flagMask);

        layout.PushFlag("flag_sixth");
        layout.PushFlag("flag_seventh");
        layout.PushFlag("flag_eighth");

        Assert.Equal(1, layout.GetDataSize());

        flagInfo = layout.GetFlagInfo("flag_sixth");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 5, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_seventh");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 6, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_eighth");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 7, flagInfo.flagMask);

        layout.PushFlag("flag_ninth");

        Assert.Equal(2, layout.GetDataSize());

        flagInfo = layout.GetFlagInfo("flag_ninth");
        Assert.Equal(1, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);
    }

    [Fact]
    public static void FlagPushEmpty()
    {
        var layout = new Layout(10);

        Assert.Equal(0, layout.GetDataSize());

        layout.PushFlag();

        Assert.Equal(1, layout.GetDataSize());

        layout.PushFlag();
        layout.PushFlag();
        layout.PushFlag();

        Assert.Equal(1, layout.GetDataSize());

        layout.PushFlag();
        layout.PushFlag();
        layout.PushFlag();
        layout.PushFlag();

        Assert.Equal(1, layout.GetDataSize());

        layout.PushFlag();

        Assert.Equal(2, layout.GetDataSize());

        layout.PushFlag("flag");

        var flagInfo = layout.GetFlagInfo("flag");
        Assert.Equal(1, flagInfo.index);
        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);
    }

    [Fact]
    public static void BytePush()
    {
        var layout = new Layout(4);

        layout.PushByte("byte_first");
        Assert.Equal(1, layout.GetDataSize());

        var byteInfo = layout.GetByteInfo("byte_first");
        Assert.Equal(0, byteInfo.index);

        layout.PushByte("byte_second");
        Assert.Equal(2, layout.GetDataSize());

        byteInfo = layout.GetByteInfo("byte_second");
        Assert.Equal(1, byteInfo.index);

        layout.PushByte("byte_third");
        layout.PushByte("byte_fourth");
        Assert.Equal(4, layout.GetDataSize());

        byteInfo = layout.GetByteInfo("byte_third");
        Assert.Equal(2, byteInfo.index);

        byteInfo = layout.GetByteInfo("byte_fourth");
        Assert.Equal(3, byteInfo.index);
    }

    [Fact]
    public static void BytePushEmpty()
    {
        var layout = new Layout(4);

        layout.PushByte();
        Assert.Equal(1, layout.GetDataSize());

        layout.PushByte();
        Assert.Equal(2, layout.GetDataSize());

        layout.PushByte();
        layout.PushByte();
        Assert.Equal(4, layout.GetDataSize());
    }

    [Fact]
    public static void DynamicPush()
    {
        var layout = new Layout(4);

        layout.PushDynamic("dynamic_byte", 1);
        Assert.Equal(1, layout.GetDataSize());

        var dynamicInfo = layout.GetDynamicInfo("dynamic_byte");
        Assert.Equal(0, dynamicInfo.index);
        Assert.Equal(1, dynamicInfo.size);

        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("dynamic_int"));

        layout.PushDynamic("dynamic_int", 4);
        Assert.Equal(5, layout.GetDataSize());

        dynamicInfo = layout.GetDynamicInfo("dynamic_int");
        Assert.Equal(1, dynamicInfo.index);
        Assert.Equal(4, dynamicInfo.size);

        layout.PushDynamic("dynamic_char", 1);
        Assert.Equal(6, layout.GetDataSize());

        dynamicInfo = layout.GetDynamicInfo("dynamic_char");
        Assert.Equal(5, dynamicInfo.index);
        Assert.Equal(1, dynamicInfo.size);
    }

    [Fact]
    public static void DynamicPushEmpty()
    {
        var layout = new Layout(4);

        layout.PushDynamic(1);
        Assert.Equal(1, layout.GetDataSize());

        layout.PushDynamic(4);
        Assert.Equal(5, layout.GetDataSize());

        layout.PushDynamic(1);
        Assert.Equal(6, layout.GetDataSize());
    }

    [Fact]
    public static void MixedPush()
    {
        var layout = new Layout(6);

        layout.PushFlag("flag_first");
        Assert.Equal(1, layout.GetDataSize());

        var flagInfo = layout.GetFlagInfo("flag_first");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

        layout.PushByte("byte_first");
        Assert.Equal(2, layout.GetDataSize());

        var byteInfo = layout.GetByteInfo("byte_first");
        Assert.Equal(1, byteInfo.index);

        layout.PushFlag("flag_second");
        layout.PushFlag("flag_third");
        Assert.Equal(3, layout.GetDataSize());

        flagInfo = layout.GetFlagInfo("flag_second");
        Assert.Equal(2, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_third");
        Assert.Equal(2, flagInfo.index);
        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);

        layout.PushDynamic("dynamic_int", 4);
        Assert.Equal(7, layout.GetDataSize());

        var dynamicInfo = layout.GetDynamicInfo("dynamic_int");
        Assert.Equal(3, dynamicInfo.index);
        Assert.Equal(4, dynamicInfo.size);

        layout.PushFlag("flag_fourth");
        Assert.Equal(8, layout.GetDataSize());

        flagInfo = layout.GetFlagInfo("flag_fourth");
        Assert.Equal(7, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

        Assert.Throws<CampaignException>(() => layout.GetByteInfo("flag_first"));
        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("flag_first"));

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("byte_first"));
        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("byte_first"));

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("dynamic_int"));
        Assert.Throws<CampaignException>(() => layout.GetByteInfo("dynamic_int"));
    }

    [Fact]
    public static void EntryExists()
    {
        var layout = new Layout(3);

        Assert.False(layout.EntryExist("flag_first"));
        layout.PushFlag("flag_first");
        Assert.True(layout.EntryExist("flag_first"));

        Assert.False(layout.EntryExist("byte_first"));
        layout.PushByte("byte_first");
        Assert.True(layout.EntryExist("byte_first"));

        Assert.False(layout.EntryExist("dynamic_int"));
        layout.PushDynamic("dynamic_int", 4);
        Assert.True(layout.EntryExist("dynamic_int"));
    }

    [Fact]
    public static void EntryExistsOfType()
    {
        var layout = new Layout(3);

        Assert.False(layout.EntryExist("flag_first", Descriptor.InfoType.Flag));
        layout.PushFlag("flag_first");
        Assert.True(layout.EntryExist("flag_first", Descriptor.InfoType.Flag));
        Assert.False(layout.EntryExist("flag_first", Descriptor.InfoType.Byte));
        Assert.False(layout.EntryExist("flag_first", Descriptor.InfoType.Dynamic));
        Assert.False(layout.EntryExist("flag_first", Descriptor.InfoType.Void));

        Assert.False(layout.EntryExist("byte_first", Descriptor.InfoType.Byte));
        layout.PushByte("byte_first");
        Assert.True(layout.EntryExist("byte_first", Descriptor.InfoType.Byte));
        Assert.False(layout.EntryExist("byte_first", Descriptor.InfoType.Flag));
        Assert.False(layout.EntryExist("byte_first", Descriptor.InfoType.Dynamic));
        Assert.False(layout.EntryExist("byte_first", Descriptor.InfoType.Void));

        Assert.False(layout.EntryExist("dynamic_int", Descriptor.InfoType.Dynamic));
        layout.PushDynamic("dynamic_int", 4);
        Assert.True(layout.EntryExist("dynamic_int", Descriptor.InfoType.Dynamic));
        Assert.False(layout.EntryExist("dynamic_int", Descriptor.InfoType.Flag));
        Assert.False(layout.EntryExist("dynamic_int", Descriptor.InfoType.Byte));
        Assert.False(layout.EntryExist("dynamic_int", Descriptor.InfoType.Void));
    }

    [Fact]
    public static void GetEntryCount()
    {
        var layout = new Layout(3);

        Assert.Equal(0, layout.GetEntryCount());

        layout.PushFlag("flag_first");
        Assert.Equal(1, layout.GetEntryCount());

        layout.PushByte("byte_first");
        Assert.Equal(2, layout.GetEntryCount());

        layout.PushDynamic("dynamic_int", 4);
        Assert.Equal(3, layout.GetEntryCount());

        layout.PushFlag();
        Assert.Equal(3, layout.GetEntryCount());

        layout.PushByte();
        Assert.Equal(3, layout.GetEntryCount());

        layout.PushDynamic(4);
        Assert.Equal(3, layout.GetEntryCount());
    }

    [Fact]
    public static void GetDataSize()
    {
        var layout = new Layout(5);

        layout.PushFlag("flag_first");
        Assert.Equal(1, layout.GetDataSize());

        layout.PushFlag("flag_second");
        Assert.Equal(1, layout.GetDataSize());

        layout.PushByte("byte_first");
        Assert.Equal(2, layout.GetDataSize());

        layout.PushFlag("flag_third");
        Assert.Equal(3, layout.GetDataSize());

        layout.PushDynamic("dynamic_int", 4);
        Assert.Equal(7, layout.GetDataSize());

        layout.PushFlag();
        Assert.Equal(8, layout.GetDataSize());

        layout.PushByte();
        Assert.Equal(9, layout.GetDataSize());

        layout.PushDynamic(4);
        Assert.Equal(13, layout.GetDataSize());
    }

    [Fact]
    public static void GetEntryInfo()
    {
        var layout = new Layout(5);

        Assert.Throws<CampaignException>(() => layout.GetEntryInfo(0));

        layout.PushFlag("flag_first");
        layout.PushFlag("flag_second");
        layout.PushByte("byte_first");
        layout.PushFlag("flag_third");
        layout.PushDynamic("dynamic_int", 4);

        var requiredIds = new HashSet<string>() { "flag_first", "flag_second", "byte_first", "flag_third", "dynamic_int" };

        for (int i = 0; i < layout.GetEntryCount(); i++)
        {
            var entryInfo = layout.GetEntryInfo(i);

            switch (entryInfo.id)
            {
                case "flag_first":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(0, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

                        requiredIds.Remove("flag_first");
                    }
                    break;

                case "flag_second":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(0, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);

                        requiredIds.Remove("flag_second");
                    }
                    break;

                case "byte_first":
                    {
                        Assert.Equal(Descriptor.InfoType.Byte, entryInfo.descriptor.type);

                        var byteInfo = entryInfo.descriptor.byteInfo;
                        Assert.Equal(1, byteInfo.index);

                        requiredIds.Remove("byte_first");
                    }
                    break;

                case "flag_third":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(2, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

                        requiredIds.Remove("flag_third");
                    }
                    break;

                case "dynamic_int":
                    {
                        Assert.Equal(Descriptor.InfoType.Dynamic, entryInfo.descriptor.type);

                        var dynamicInfo = entryInfo.descriptor.dynamicInfo;
                        Assert.Equal(3, dynamicInfo.index);
                        Assert.Equal(4, dynamicInfo.size);

                        requiredIds.Remove("dynamic_int");
                    }
                    break;

                default:
                    Assert.Fail($"Didn't expect an entry of id \"{entryInfo.id}\".");
                    break;
            }
        }
    
        Assert.Empty(requiredIds);

        Assert.Throws<CampaignException>(() => layout.GetEntryInfo(5));
    }

    [Fact]
    public static void GetFlagInfo()
    {
        var layout = new Layout(5);

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("flag_first"));
        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("flag_second"));

        layout.PushFlag("flag_first");

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("flag_second"));

        layout.PushFlag("flag_second");

        var flagInfo = layout.GetFlagInfo("flag_first");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

        flagInfo = layout.GetFlagInfo("flag_second");
        Assert.Equal(0, flagInfo.index);
        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);

        layout.PushByte();
        layout.PushFlag("flag_third");

        flagInfo = layout.GetFlagInfo("flag_third");
        Assert.Equal(2, flagInfo.index);
        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

        layout.PushByte("byte");
        layout.PushDynamic("dynamic", 4);

        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("byte"));
        Assert.Throws<CampaignException>(() => layout.GetFlagInfo("dynamic"));
    }

    [Fact]
    public static void GetByteInfo()
    {
        var layout = new Layout(4);

        Assert.Throws<CampaignException>(() => layout.GetByteInfo("byte_first"));
        Assert.Throws<CampaignException>(() => layout.GetByteInfo("byte_second"));

        layout.PushByte("byte_first");

        Assert.Throws<CampaignException>(() => layout.GetByteInfo("byte_second"));

        layout.PushByte("byte_second");

        var byteInfo = layout.GetByteInfo("byte_first");
        Assert.Equal(0, byteInfo.index);

        byteInfo = layout.GetByteInfo("byte_second");
        Assert.Equal(1, byteInfo.index);

        layout.PushFlag("flag");
        layout.PushDynamic("dynamic", 4);

        Assert.Throws<CampaignException>(() => layout.GetByteInfo("flag"));
        Assert.Throws<CampaignException>(() => layout.GetByteInfo("dynamic"));
    }

    [Fact]
    public static void GetDynamicInfo()
    {
        var layout = new Layout(4);

        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("dynamic_int"));
        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("dynamic_double"));

        layout.PushDynamic("dynamic_int", 4);

        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("dynamic_double"));

        layout.PushDynamic("dynamic_double", 8);

        var dynamicInfo = layout.GetDynamicInfo("dynamic_int");
        Assert.Equal(0, dynamicInfo.index);
        Assert.Equal(4, dynamicInfo.size);

        dynamicInfo = layout.GetDynamicInfo("dynamic_double");
        Assert.Equal(4, dynamicInfo.index);
        Assert.Equal(8, dynamicInfo.size);

        layout.PushFlag("flag");
        layout.PushByte("byte");

        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("flag"));
        Assert.Throws<CampaignException>(() => layout.GetDynamicInfo("byte"));
    }

    [Fact]
    public static void Enumerator()
    {
        var layout = new Layout(5);

        Assert.Throws<CampaignException>(() => layout.GetEntryInfo(0));

        layout.PushFlag("flag_first");
        layout.PushFlag("flag_second");
        layout.PushByte("byte_first");
        layout.PushFlag("flag_third");
        layout.PushDynamic("dynamic_int", 4);

        var requiredIds = new HashSet<string>() { "flag_first", "flag_second", "byte_first", "flag_third", "dynamic_int" };

        foreach (var entryInfo in layout)
        {
            switch (entryInfo.id)
            {
                case "flag_first":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(0, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

                        requiredIds.Remove("flag_first");
                    }
                    break;

                case "flag_second":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(0, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 1, flagInfo.flagMask);

                        requiredIds.Remove("flag_second");
                    }
                    break;

                case "byte_first":
                    {
                        Assert.Equal(Descriptor.InfoType.Byte, entryInfo.descriptor.type);

                        var byteInfo = entryInfo.descriptor.byteInfo;
                        Assert.Equal(1, byteInfo.index);

                        requiredIds.Remove("byte_first");
                    }
                    break;

                case "flag_third":
                    {
                        Assert.Equal(Descriptor.InfoType.Flag, entryInfo.descriptor.type);

                        var flagInfo = entryInfo.descriptor.flagInfo;
                        Assert.Equal(2, flagInfo.index);
                        Assert.Equal<byte>(0x1 << 0, flagInfo.flagMask);

                        requiredIds.Remove("flag_third");
                    }
                    break;

                case "dynamic_int":
                    {
                        Assert.Equal(Descriptor.InfoType.Dynamic, entryInfo.descriptor.type);

                        var dynamicInfo = entryInfo.descriptor.dynamicInfo;
                        Assert.Equal(3, dynamicInfo.index);
                        Assert.Equal(4, dynamicInfo.size);

                        requiredIds.Remove("dynamic_int");
                    }
                    break;

                default:
                    Assert.Fail($"Didn't expect an entry of id \"{entryInfo.id}\".");
                    break;
            }
        }
    
        Assert.Empty(requiredIds);

        Assert.Throws<CampaignException>(() => layout.GetEntryInfo(5));
    }
}