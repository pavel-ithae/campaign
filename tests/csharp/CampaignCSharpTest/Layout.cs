using System.Runtime.InteropServices;

namespace Campaign
{
    public enum APIResult
    {
        Success = 0,
        Failure = -1
    }

    public static class LayoutAPI
    {
        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern APIResult campaign_layout_create(int descriptorCount, out IntPtr layoutPtr);

        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern APIResult campaign_layout_delete(IntPtr layoutPtr);

        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern APIResult campaign_layout_push_flag(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id);
    }
}