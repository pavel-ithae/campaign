namespace Campaign;


public class CampaignException : Exception
{
    public CampaignException()
        : base()
    {
    }

    public CampaignException(string? message)
        : base(message)
    {
    }

    public CampaignException(string? message, Exception? innerException)
        : base(message, innerException)
    {
    }
}