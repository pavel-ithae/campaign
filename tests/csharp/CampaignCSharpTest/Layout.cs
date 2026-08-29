using System.Runtime.InteropServices;

namespace Campaign
{
    public static class LayoutAPI
    {
        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr campaign_layout_create(int descriptorCount);

        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern void campaign_layout_delete(IntPtr layoutPtr);

        [DllImport("libs/libcampaign", CallingConvention = CallingConvention.Cdecl)]
        public static extern void campaign_layout_push_flag(IntPtr layoutPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id);
    }
}