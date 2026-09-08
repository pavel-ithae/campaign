namespace Campaign.API;

using System.Runtime.InteropServices;

internal static partial class LayoutAPI
{
    // NOTE: ref struct will have compatibility issues with Unity (Net4.8). Find a solution later.

    [StructLayout(LayoutKind.Sequential)]
    internal ref struct LayoutEntryNative : IDisposable
    {
        public IntPtr idPtr;

        public Descriptor descriptor;


        public string? id => (idPtr == IntPtr.Zero) ? null : Marshal.PtrToStringUTF8(idPtr);


        public static explicit operator LayoutEntry(LayoutEntryNative native)
        {
            return new LayoutEntry()
            {
                id = native.id,
                descriptor = native.descriptor
            };
        }


        public void Dispose()
        {
            if (idPtr == IntPtr.Zero)
            {
                return;
            }

            StringAPI.Free(idPtr);

            idPtr = IntPtr.Zero;
        }
    }

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_create")]
    internal static partial APIResult Create(int descriptorCount, out IntPtr layoutPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_delete")]
    internal static partial APIResult Delete(IntPtr layoutPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_flag")]
    internal static partial APIResult PushFlag(IntPtr layoutPtr, string id);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_flag_empty")]
    internal static partial APIResult PushFlag(IntPtr layoutPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_byte")]
    internal static partial APIResult PushByte(IntPtr layoutPtr, string id);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_byte_empty")]
    internal static partial APIResult PushByte(IntPtr layoutPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_dynamic")]
    internal static partial APIResult PushDynamic(IntPtr layoutPtr, string id, int size);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_push_dynamic_empty")]
    internal static partial APIResult PushDynamic(IntPtr layoutPtr, int size);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_entry_exists")]
    internal static partial APIResult EntryExists(IntPtr layoutPtr, string id, [MarshalAs(UnmanagedType.U1)] out bool exists);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_entry_count")]
    internal static partial APIResult GetEntryCount(IntPtr layoutPtr, out int entryCount);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_data_size")]
    internal static partial APIResult GetDataSize(IntPtr layoutPtr, out int dataSize);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_entry_info")]
    internal static partial APIResult GetEntryInfo(IntPtr layoutPtr, int index, out LayoutEntryNative entry);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_descriptor")]
    internal static partial APIResult GetDescriptor(IntPtr layoutPtr, string id, out Descriptor descriptor);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_flag_info")]
    internal static partial APIResult GetFlagInfo(IntPtr layoutPtr, string id, out Descriptor.FlagInfo flagInfo);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_byte_info")]
    internal static partial APIResult GetByteInfo(IntPtr layoutPtr, string id, out Descriptor.ByteInfo byteInfo);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_dynamic_info")]
    internal static partial APIResult GetDynamicInfo(IntPtr layoutPtr, string id, out Descriptor.DynamicInfo dynamicInfo);
}
