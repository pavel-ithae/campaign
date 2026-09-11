using System;
using System.Runtime.InteropServices;

#if !NET48
using System.Runtime.InteropServices.Marshalling;
#endif


namespace Campaign.API
{
    internal static partial class LayoutAPI
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct LayoutEntryInfoNative
        {
            public IntPtr idPtr;

            public Descriptor descriptor;


            public Layout.EntryInfo CopyToManaged()
            {
#if !NET48
                string? id = Marshal.PtrToStringUTF8(idPtr);
#else
                string id = APIUtility.PtrToStringUTF8(idPtr);
#endif

                return new Layout.EntryInfo(
                    ((idPtr == IntPtr.Zero) ? null : id) ?? string.Empty,
                    descriptor
                    );
            }

            public void Free()
            {
                if (idPtr == IntPtr.Zero)
                {
                    return;
                }

                StringAPI.Free(idPtr);
            }
        }

#if !NET48
        [CustomMarshaller(typeof(Layout.EntryInfo), MarshalMode.Default, typeof(LayoutEntryInfoMarshaller))]
#endif
        internal static class LayoutEntryInfoMarshaller
        {
            public static Layout.EntryInfo ConvertToManaged(LayoutEntryInfoNative unmanaged)
            {
                return unmanaged.CopyToManaged();
            }

            public static void Free(LayoutEntryInfoNative unmanaged)
            {
                unmanaged.Free();
            }
        }

#if !NET48
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

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_entry_exists_of_type")]
        internal static partial APIResult EntryExists(IntPtr layoutPtr, string id, Descriptor.InfoType infoType, [MarshalAs(UnmanagedType.U1)] out bool exists);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_entry_count")]
        internal static partial APIResult GetEntryCount(IntPtr layoutPtr, out int entryCount);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_data_size")]
        internal static partial APIResult GetDataSize(IntPtr layoutPtr, out int dataSize);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_entry_info")]
        internal static partial APIResult GetEntryInfo(IntPtr layoutPtr, int index, out Layout.EntryInfo entryInfo);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_descriptor")]
        internal static partial APIResult GetDescriptor(IntPtr layoutPtr, string id, out Descriptor descriptor);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_flag_info")]
        internal static partial APIResult GetFlagInfo(IntPtr layoutPtr, string id, out Descriptor.FlagInfo flagInfo);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_byte_info")]
        internal static partial APIResult GetByteInfo(IntPtr layoutPtr, string id, out Descriptor.ByteInfo byteInfo);

        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_layout_get_dynamic_info")]
        internal static partial APIResult GetDynamicInfo(IntPtr layoutPtr, string id, out Descriptor.DynamicInfo dynamicInfo);
#else
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
#endif
    }
}