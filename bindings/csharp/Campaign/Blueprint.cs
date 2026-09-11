namespace Campaign;

using System.Collections;
using System.Runtime.InteropServices.Marshalling;
using Campaign.API;


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
            BlueprintAPI.IteratorNext(_blueprintPtr, ref iteratorPtr, out var pieceInfo).ValidateAPICall();

            yield return pieceInfo;
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

    
    [NativeMarshalling(typeof(BlueprintAPI.BlueprintPieceInfoMarshaller))]
    public readonly struct PieceInfo
    {
        public readonly string id;

        public readonly Layout? layout;


        public PieceInfo(string id, Layout? layout)
        {
            this.id = id;
            this.layout = layout;
        }
    }
}