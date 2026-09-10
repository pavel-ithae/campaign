using System;


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

        unsafe internal static string PtrToStringUTF8(IntPtr stringPtr)
        {
            if (stringPtr == IntPtr.Zero)
            {
                return null;
            }

            byte* charUTF8Ptr = (byte*)stringPtr.ToPointer();
            int length = 0;

            while (charUTF8Ptr[length] != 0)
            {
                length++;
            }

            return System.Text.Encoding.UTF8.GetString(charUTF8Ptr, length);
        }
    }
}