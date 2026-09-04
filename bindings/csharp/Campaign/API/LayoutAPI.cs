namespace Campaign.API;

using System.Runtime.InteropServices;

public static partial class LayoutAPI
{
    // TODO: Make these internal and create a public Layout class.

    [LibraryImport("libs/libcampaign", StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_create")]
    public static partial APIResult Create(int descriptorCount, out IntPtr layoutPtr);

    [LibraryImport("libs/libcampaign", StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_delete")]
    public static partial APIResult Delete(IntPtr layoutPtr);

    [LibraryImport("libs/libcampaign", StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_flag")]
    public static partial APIResult PushFlag(IntPtr layoutPtr, string id);

    [LibraryImport("libs/libcampaign", StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_entry_exists")]
    public static partial APIResult EntryExists(IntPtr layoutPtr, string id, [MarshalAs(UnmanagedType.U1)] out bool exists);
}
