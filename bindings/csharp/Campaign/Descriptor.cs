using System.Runtime.InteropServices;

namespace Campaign
{
    [StructLayout(LayoutKind.Sequential)]
    public readonly struct Descriptor
    {
        public readonly InfoType type;

        private readonly int _index;

        private readonly byte _flagMask;

        private readonly int _size;


        public FlagInfo flagInfo
        {
            get
            {
                if (type != InfoType.Flag)
                {
                    throw new CampaignException($"Tried to get a {nameof(FlagInfo)} from a {nameof(Descriptor)} of type {type}");
                }

                return new FlagInfo(_index, _flagMask);
            }
        }

        public ByteInfo byteInfo
        {
            get
            {
                if (type != InfoType.Byte)
                {
                    throw new CampaignException($"Tried to get a {nameof(ByteInfo)} from a {nameof(Descriptor)} of type {type}");
                }

                return new ByteInfo(_index);
            }
        }

        public DynamicInfo dynamicInfo
        {
            get
            {
                if (type != InfoType.Dynamic)
                {
                    throw new CampaignException($"Tried to get a {nameof(DynamicInfo)} from a {nameof(Descriptor)} of type {type}");
                }

                return new DynamicInfo(_index, _size);
            }
        }


        public override string ToString()
        {
            switch (type)
            {
                case InfoType.Void:
                    return "Void{}";

                case InfoType.Flag:
                    return FlagInfo.BuildString(_index, _flagMask);

                case InfoType.Byte:
                    return ByteInfo.BuildString(_index);

                case InfoType.Dynamic:
                    return DynamicInfo.BuildString(_index, _size);

                default:
                    return "Unknown{}";
            }
        }


        public enum InfoType
        {
            Void,
            Flag,
            Byte,
            Dynamic
        }

        [StructLayout(LayoutKind.Sequential)]
        public readonly struct FlagInfo
        {
            public readonly int index;

            public readonly byte flagMask;


            internal FlagInfo(int index, byte flagMask)
            {
                this.index = index;
                this.flagMask = flagMask;
            }


            public override string ToString()
            {
                return BuildString(index, flagMask);
            }

            internal static string BuildString(int index, byte flagMask)
            {
                return $"Flag{{{index},{flagMask}}}";
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public readonly struct ByteInfo
        {
            public readonly int index;


            internal ByteInfo(int index)
            {
                this.index = index;
            }


            public override string ToString()
            {
                return BuildString(index);
            }

            internal static string BuildString(int index)
            {
                return $"Byte{{{index}}}";
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public readonly struct DynamicInfo
        {
            public readonly int index;

            public readonly int size;


            internal DynamicInfo(int index, int size)
            {
                this.index = index;
                this.size = size;
            }


            public override string ToString()
            {
                return BuildString(index, size);
            }

            internal static string BuildString(int index, int size)
            {
                return $"Dynamic{{{index},{size}}}";
            }
        }
    }
}