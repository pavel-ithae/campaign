namespace Campaign.API;

using System.Runtime.InteropServices;


internal static partial class BlueprintAPI
{
    [StructLayout(LayoutKind.Sequential)]
    internal ref struct BlueprintPieceInfoNative : IDisposable
    {
        public IntPtr idPtr;

        public IntPtr layoutPtr;


        private string id => ((idPtr == IntPtr.Zero) ? null : Marshal.PtrToStringUTF8(idPtr)) ?? string.Empty;

        private Layout? layout => (layoutPtr == IntPtr.Zero) ? null : new Layout(layoutPtr);


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


    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_create")]
    internal static partial APIResult Create(int pieceCount, out IntPtr blueprintPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_delete")]
    internal static partial APIResult Delete(IntPtr blueprintPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_push")]
    internal static partial APIResult Push(IntPtr blueprintPtr, string id, IntPtr layoutPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_get_size")]
    internal static partial APIResult GetSize(IntPtr blueprintPtr, out int size);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_get_iterator")]
    internal static partial APIResult GetIterator(IntPtr blueprintPtr, out IntPtr iteratorPtr);

    [LibraryImport(APIUtility.LIBRARY_NAME, StringMarshalling = StringMarshalling.Utf8, EntryPoint = "campaign_blueprint_iterator_next")]
    internal static partial APIResult IteratorNext(IntPtr blueprintPtr, ref IntPtr iteratorPtr, out BlueprintPieceInfoNative current);
}