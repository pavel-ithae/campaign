namespace Campaign;

using Campaign.API;

public class Layout
{
    private IntPtr _layoutPtr;


    public Layout(int descriptorCount)
    {
        LayoutAPI.Create(descriptorCount, out _layoutPtr).ValidateAPICall();
    }

    ~Layout()
    {
        LayoutAPI.Delete(_layoutPtr).ValidateAPICall();
    }


    public void PushFlag(string id)
    {
        LayoutAPI.PushFlag(_layoutPtr, id).ValidateAPICall();
    }

    public void PushFlag()
    {
        LayoutAPI.PushFlag(_layoutPtr).ValidateAPICall();
    }

    public void PushByte(string id)
    {
        LayoutAPI.PushByte(_layoutPtr, id).ValidateAPICall();
    }

    public void PushByte()
    {
        LayoutAPI.PushByte(_layoutPtr).ValidateAPICall();
    }

    public void PushDynamic(string id, int size)
    {
        LayoutAPI.PushDynamic(_layoutPtr, id, size).ValidateAPICall();
    }

    public void PushDynamic(int size)
    {
        LayoutAPI.PushDynamic(_layoutPtr, size).ValidateAPICall();
    }

    public bool EntryExist(string id)
    {
        LayoutAPI.EntryExists(_layoutPtr, id, out bool exists).ValidateAPICall();

        return exists;
    }

    public int GetEntryCount()
    {
        LayoutAPI.GetEntryCount(_layoutPtr, out var entryCount).ValidateAPICall();

        return entryCount;
    }

    public int GetDataSize()
    {
        LayoutAPI.GetDataSize(_layoutPtr, out var dataSize).ValidateAPICall();

        return dataSize;
    }

    public LayoutEntry GetEntryInfo(int index)
    {
        LayoutAPI.GetEntryInfo(_layoutPtr, index, out var nativeEntry).ValidateAPICall();

        try
        {
            return (LayoutEntry)nativeEntry;
        }
        finally
        {
            nativeEntry.Dispose();
        }
    }

    public Descriptor GetDescriptor(string id)
    {
        LayoutAPI.GetDescriptor(_layoutPtr, id, out var descriptor).ValidateAPICall();

        return descriptor;
    }

    public Descriptor.FlagInfo GetFlagInfo(string id)
    {
        LayoutAPI.GetFlagInfo(_layoutPtr, id, out var flagInfo).ValidateAPICall();

        return flagInfo;
    }

    public Descriptor.ByteInfo GetByteInfo(string id)
    {
        LayoutAPI.GetByteInfo(_layoutPtr, id, out var byteInfo).ValidateAPICall();

        return byteInfo;
    }

    public Descriptor.DynamicInfo GetDynamicInfo(string id)
    {
        LayoutAPI.GetDynamicInfo(_layoutPtr, id, out var dynamicInfo).ValidateAPICall();

        return dynamicInfo;
    }
}