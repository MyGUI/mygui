using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct FloatSize
    {
        [MarshalAs(UnmanagedType.R4)]
        public float width;
        [MarshalAs(UnmanagedType.R4)]
        public float height;

        public FloatSize(float _width, float _height)
        {
            width = _width;
            height = _height;
        }
    }
}
