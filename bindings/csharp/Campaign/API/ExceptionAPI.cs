using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static partial class ExceptionAPI
    {
#if !NET48
        [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_exception_get_last_message")]
        internal static partial string GetLastExceptionMessage();
#else
        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_exception_get_last_message")]
        [return: MarshalAs(UnmanagedType.LPUTF8Str)]
        internal static extern string GetLastExceptionMessage();
#endif
    }
}