namespace Campaign.API
{
    internal static class APIUtility
    {
        internal const string LIBRARY_NAME = "libs/libcampaign";

        internal static void ValidateAPICall(this APIResult apiCallResult)
        {
            if (apiCallResult == APIResult.Failure)
            {
                throw new CampaignException(ExceptionAPI.GetLastExceptionMessage());
            }
        }

#if NET48
        internal unsafe static string PtrToStringUTF8(System.IntPtr stringPtr)
        {
            byte* utf8Ptr = (byte*)stringPtr.ToPointer();
            int length = 0;

            while (utf8Ptr[length] != 0)
            {
                length++;
            }

            return System.Text.Encoding.UTF8.GetString(utf8Ptr, length);
        }
#endif
    }
}