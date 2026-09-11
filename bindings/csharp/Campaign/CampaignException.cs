using System;


namespace Campaign
{
    public class CampaignException : Exception
    {
        public CampaignException()
            : base()
        {
        }

#if !NET48
        public CampaignException(string? message)
#else
        public CampaignException(string message)
#endif
            : base(message)
        {
        }

#if !NET48
        public CampaignException(string? message, Exception? innerException)
#else
        public CampaignException(string message, Exception innerException)
#endif
            : base(message, innerException)
        {
        }
    }
}