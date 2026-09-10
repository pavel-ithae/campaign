using System;
using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static class LayoutAPI
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct LayoutEntryInfoNative : IDisposable
        {
            public IntPtr idPtr;

            public Descriptor descriptor;


            private string id => ((idPtr == IntPtr.Zero) ? null : APIUtility.PtrToStringUTF8(idPtr)) ?? string.Empty;


            public Layout.EntryInfo TransferToManaged()
            {
                try
                {
                    return new Layout.EntryInfo(id ?? string.Empty, descriptor);
                }
                finally
                {
                    StringAPI.Free(idPtr); // String was copied.
                    idPtr = IntPtr.Zero;
                }
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

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_create")]
        internal static extern APIResult Create(int descriptorCount, out IntPtr layoutPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_delete")]
        internal static extern APIResult Delete(IntPtr layoutPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_flag")]
        internal static extern APIResult PushFlag(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_flag_empty")]
        internal static extern APIResult PushFlag(IntPtr layoutPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_byte")]
        internal static extern APIResult PushByte(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_byte_empty")]
        internal static extern APIResult PushByte(IntPtr layoutPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_dynamic")]
        internal static extern APIResult PushDynamic(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, int size);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_push_dynamic_empty")]
        internal static extern APIResult PushDynamic(IntPtr layoutPtr, int size);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_entry_exists")]
        internal static extern APIResult EntryExists(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, [MarshalAs(UnmanagedType.U1)] out bool exists);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_entry_exists_of_type")]
        internal static extern APIResult EntryExists(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, Descriptor.InfoType infoType, [MarshalAs(UnmanagedType.U1)] out bool exists);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_entry_count")]
        internal static extern APIResult GetEntryCount(IntPtr layoutPtr, out int entryCount);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_data_size")]
        internal static extern APIResult GetDataSize(IntPtr layoutPtr, out int dataSize);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_entry_info")]
        internal static extern APIResult GetEntryInfo(IntPtr layoutPtr, int index, out LayoutEntryInfoNative entryInfo);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_descriptor")]
        internal static extern APIResult GetDescriptor(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, out Descriptor descriptor);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_flag_info")]
        internal static extern APIResult GetFlagInfo(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, out Descriptor.FlagInfo flagInfo);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_byte_info")]
        internal static extern APIResult GetByteInfo(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, out Descriptor.ByteInfo byteInfo);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_layout_get_dynamic_info")]
        internal static extern APIResult GetDynamicInfo(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, out Descriptor.DynamicInfo dynamicInfo);
    }
}