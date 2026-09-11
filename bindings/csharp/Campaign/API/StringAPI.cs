using System;
using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static partial class StringAPI
    {
#if !NET48
        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_string_free")]
        internal static partial APIResult Free(IntPtr stringPtr);
#else
        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_string_free")]
        internal static extern APIResult Free(IntPtr stringPtr);
#endif
    }
}