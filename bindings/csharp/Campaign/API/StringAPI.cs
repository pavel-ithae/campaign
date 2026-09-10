using System;
using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static class StringAPI
    {
        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_string_free")]
        internal static extern APIResult Free(IntPtr stringPtr);
    }
}