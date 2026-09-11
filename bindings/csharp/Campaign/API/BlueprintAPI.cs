namespace Campaign.API;

using System.Runtime.InteropServices;
using System.Runtime.InteropServices.Marshalling;


internal static partial class BlueprintAPI
{
    [CustomMarshaller(typeof(Blueprint.PieceInfo), MarshalMode.ManagedToUnmanagedOut, typeof(BlueprintPieceInfoMarshaller))]
    internal static class BlueprintPieceInfoMarshaller
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct BlueprintPieceInfoNative
        {
            public IntPtr idPtr;

            public IntPtr layoutPtr;


            public Blueprint.PieceInfo CopyToManaged()
            {
                return new Blueprint.PieceInfo(
                    ((idPtr == IntPtr.Zero) ? null : Marshal.PtrToStringUTF8(idPtr)) ?? string.Empty,
                    (layoutPtr == IntPtr.Zero) ? null : new Layout(layoutPtr)
                    );
            }

            public void Free()
            {
                if (idPtr == IntPtr.Zero)
                {
                    return;
                }

                StringAPI.Free(idPtr);
            }
        }


        public static Blueprint.PieceInfo ConvertToManaged(BlueprintPieceInfoNative unmanaged)
        {
            return unmanaged.CopyToManaged();
        }

        public static void Free(BlueprintPieceInfoNative unmanaged)
        {
            unmanaged.Free();
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
    internal static partial APIResult IteratorNext(IntPtr blueprintPtr, ref IntPtr iteratorPtr, out Blueprint.PieceInfo current);
}