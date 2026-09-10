using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Campaign.API;

namespace Campaign
{
    public class Layout : IDisposable, IEnumerable<Layout.EntryInfo>
    {
        private IntPtr _layoutPtr;


        public Layout(int descriptorCount)
        {
            LayoutAPI.Create(descriptorCount, out _layoutPtr).ValidateAPICall();
        }

        internal Layout(IntPtr layoutPtr)
        {
            _layoutPtr = layoutPtr;
        }

        ~Layout()
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


        internal IntPtr ptr => _layoutPtr;


        public void PushFlag(string id)
        {
            LayoutAPI.PushFlag(_layoutPtr, id).ValidateAPICall();
        }

        public void PushFlag()
        {
            LayoutAPI.PushFlag(_layoutPtr).ValidateAPICall();
        }

        public void PushByte(string id)
        {
            LayoutAPI.PushByte(_layoutPtr, id).ValidateAPICall();
        }

        public void PushByte()
        {
            LayoutAPI.PushByte(_layoutPtr).ValidateAPICall();
        }

        public void PushDynamic(string id, int size)
        {
            LayoutAPI.PushDynamic(_layoutPtr, id, size).ValidateAPICall();
        }

        public void PushDynamic(int size)
        {
            LayoutAPI.PushDynamic(_layoutPtr, size).ValidateAPICall();
        }

        public bool EntryExist(string id)
        {
            LayoutAPI.EntryExists(_layoutPtr, id, out bool exists).ValidateAPICall();

            return exists;
        }

        public bool EntryExist(string id, Descriptor.InfoType infoType)
        {
            LayoutAPI.EntryExists(_layoutPtr, id, infoType, out bool exists).ValidateAPICall();

            return exists;
        }

        public int GetEntryCount()
        {
            LayoutAPI.GetEntryCount(_layoutPtr, out var entryCount).ValidateAPICall();

            return entryCount;
        }

        public int GetDataSize()
        {
            LayoutAPI.GetDataSize(_layoutPtr, out var dataSize).ValidateAPICall();

            return dataSize;
        }

        public EntryInfo GetEntryInfo(int index)
        {
            LayoutAPI.GetEntryInfo(_layoutPtr, index, out var nativeEntry).ValidateAPICall();

            try
            {
                return nativeEntry.TransferToManaged();
            }
            finally
            {
                nativeEntry.Dispose();
            }
        }

        public Descriptor GetDescriptor(string id)
        {
            LayoutAPI.GetDescriptor(_layoutPtr, id, out var descriptor).ValidateAPICall();

            return descriptor;
        }

        public Descriptor.FlagInfo GetFlagInfo(string id)
        {
            LayoutAPI.GetFlagInfo(_layoutPtr, id, out var flagInfo).ValidateAPICall();

            return flagInfo;
        }

        public Descriptor.ByteInfo GetByteInfo(string id)
        {
            LayoutAPI.GetByteInfo(_layoutPtr, id, out var byteInfo).ValidateAPICall();

            return byteInfo;
        }

        public Descriptor.DynamicInfo GetDynamicInfo(string id)
        {
            LayoutAPI.GetDynamicInfo(_layoutPtr, id, out var dynamicInfo).ValidateAPICall();

            return dynamicInfo;
        }

        public IEnumerator<EntryInfo> GetEnumerator()
        {
            var count = GetEntryCount();
            for (int i = 0; i < count; i++)
            {
                yield return GetEntryInfo(i);
            }
        }

        private void DisposeInternal()
        {
            if (_layoutPtr == IntPtr.Zero)
            {
                return;
            }

            LayoutAPI.Delete(_layoutPtr).ValidateAPICall();

            _layoutPtr = IntPtr.Zero;
        }


        [StructLayout(LayoutKind.Sequential)]
        public readonly struct EntryInfo
        {
            public readonly string id;

            public readonly Descriptor descriptor;


            public EntryInfo(in string id, in Descriptor descriptor)
            {
                this.id = id;
                this.descriptor = descriptor;
            }
        }
    }
}