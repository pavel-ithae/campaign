using System;
using System.Runtime.InteropServices;


namespace Campaign.API
{
    internal static class BlueprintAPI
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct BlueprintPieceInfoNative : IDisposable
        {
            public IntPtr idPtr;

            public IntPtr layoutPtr;


            private string id => ((idPtr == IntPtr.Zero) ? null : APIUtility.PtrToStringUTF8(idPtr)) ?? string.Empty;

            private Layout layout => (layoutPtr == IntPtr.Zero) ? null : new Layout(layoutPtr);


            public Blueprint.PieceInfo TransferToManaged()
            {
                try
                {
                    return new Blueprint.PieceInfo(id ?? string.Empty, layout);
                }
                finally
                {
                    StringAPI.Free(idPtr); // String was copied.
                    idPtr = IntPtr.Zero;

                    layoutPtr = IntPtr.Zero; // Layout was transfered.
                }
            }

            public void Dispose()
            {
                if (idPtr != IntPtr.Zero)
                {
                    StringAPI.Free(idPtr);
                    idPtr = IntPtr.Zero;
                }

                if (layoutPtr != IntPtr.Zero)
                {
                    LayoutAPI.Delete(layoutPtr);
                    layoutPtr = IntPtr.Zero;
                }
            }
        }


        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_create")]
        internal static extern APIResult Create(int pieceCount, out IntPtr blueprintPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_delete")]
        internal static extern APIResult Delete(IntPtr blueprintPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_push")]
        internal static extern APIResult Push(IntPtr blueprintPtr, [MarshalAs(UnmanagedType.LPUTF8Str)] string id, IntPtr layoutPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_get_size")]
        internal static extern APIResult GetSize(IntPtr blueprintPtr, out int size);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_get_iterator")]
        internal static extern APIResult GetIterator(IntPtr blueprintPtr, out IntPtr iteratorPtr);

        [DllImport(APIUtility.LIBRARY_NAME, EntryPoint = "campaign_blueprint_iterator_next")]
        internal static extern APIResult IteratorNext(IntPtr blueprintPtr, ref IntPtr iteratorPtr, out BlueprintPieceInfoNative current);
    }
}