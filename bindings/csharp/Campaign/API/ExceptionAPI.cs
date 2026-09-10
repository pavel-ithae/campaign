using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static partial class ExceptionAPI
    {
        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_exception_get_last_message")]
        [return: MarshalAs(UnmanagedType.LPUTF8Str)]
        internal static extern string GetLastExceptionMessage();
    }
}