using System;
using System.Collections;
using System.Collections.Generic;

#if !NET48
using System.Runtime.InteropServices.Marshalling;
#endif

using Campaign.API;


namespace Campaign
{
    public class Blueprint : IDisposable, IEnumerable<Blueprint.PieceInfo>
    {
        private IntPtr _blueprintPtr;


        public Blueprint(int pieceCount)
        {
            BlueprintAPI.Create(pieceCount, out _blueprintPtr).ValidateAPICall();
        }

        ~Blueprint()
        {
            DisposeInternal();
        }


        void IDisposable.Dispose()
        {
            DisposeInternal();
            GC.SuppressFinalize(this);
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }


        public int count
        {
            get
            {
                BlueprintAPI.GetSize(_blueprintPtr, out var value);

                return value;
            }
        }


        public void Push(string id, Layout layout)
        {
            BlueprintAPI.Push(_blueprintPtr, id, layout.ptr).ValidateAPICall();
        }

        public IEnumerator<PieceInfo> GetEnumerator()
        {
            BlueprintAPI.GetIterator(_blueprintPtr, out var iteratorPtr).ValidateAPICall();

            int size = count;
            for (int i = 0; i < size; i++)
            {
#if !NET48
                BlueprintAPI.IteratorNext(_blueprintPtr, ref iteratorPtr, out var pieceInfo).ValidateAPICall();

                yield return pieceInfo;
#else
                BlueprintAPI.IteratorNext(_blueprintPtr, ref iteratorPtr, out var pieceInfoNative).ValidateAPICall();

                try
                {
                    yield return BlueprintAPI.BlueprintPieceInfoMarshaller.ConvertToManaged(pieceInfoNative);
                }
                finally
                {
                    BlueprintAPI.BlueprintPieceInfoMarshaller.Free(pieceInfoNative);
                }
#endif
            }
        }

        private void DisposeInternal()
        {
            if (_blueprintPtr == IntPtr.Zero)
            {
                return;
            }

            LayoutAPI.Delete(_blueprintPtr).ValidateAPICall();

            _blueprintPtr = IntPtr.Zero;
        }

#if !NET48
        [NativeMarshalling(typeof(BlueprintAPI.BlueprintPieceInfoMarshaller))]
#endif
        public readonly struct PieceInfo
        {
            public readonly string id;
#if !NET48
            public readonly Layout? layout;
#else
            public readonly Layout layout;
#endif


#if !NET48
            public PieceInfo(string id, Layout? layout)
#else
            public PieceInfo(string id, Layout layout)
#endif
            {
                this.id = id;
                this.layout = layout;
            }
        }
    }
}