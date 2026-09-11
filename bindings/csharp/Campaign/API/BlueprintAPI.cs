using System;
using System.Runtime.InteropServices;

#if !NET48
using System.Runtime.InteropServices.Marshalling;
#endif


namespace Campaign.API
{
    internal static partial class BlueprintAPI
    {
        [StructLayout(LayoutKind.Sequential)]
        internal struct BlueprintPieceInfoNative
        {
            public IntPtr idPtr;

            public IntPtr layoutPtr;


            public Blueprint.PieceInfo CopyToManaged()
            {
#if !NET48
                string? id = Marshal.PtrToStringUTF8(idPtr);
#else
                string id = APIUtility.PtrToStringUTF8(idPtr);
#endif

                return new Blueprint.PieceInfo(
                    ((idPtr == IntPtr.Zero) ? null : id) ?? string.Empty,
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

#if !NET48
        [CustomMarshaller(typeof(Blueprint.PieceInfo), MarshalMode.ManagedToUnmanagedOut, typeof(BlueprintPieceInfoMarshaller))]
#endif
        internal static class BlueprintPieceInfoMarshaller
        {
            public static Blueprint.PieceInfo ConvertToManaged(BlueprintPieceInfoNative unmanaged)
            {
                return unmanaged.CopyToManaged();
            }

            public static void Free(BlueprintPieceInfoNative unmanaged)
            {
                unmanaged.Free();
            }
        }


#if !NET48
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
#else
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
#endif
    }
}