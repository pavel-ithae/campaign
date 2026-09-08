namespace Campaign.API;

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
}