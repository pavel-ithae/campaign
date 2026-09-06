namespace Campaign.API;

using System.Runtime.InteropServices;

internal static partial class StringAPI
{
    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_string_free")]
    internal static partial APIResult Free(IntPtr stringPtr);
}