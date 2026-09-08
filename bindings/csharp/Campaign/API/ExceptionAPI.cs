namespace Campaign.API;

using System.Runtime.InteropServices;


internal static partial class ExceptionAPI
{
    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_exception_get_last_message")]
    internal static partial string GetLastExceptionMessage();
}