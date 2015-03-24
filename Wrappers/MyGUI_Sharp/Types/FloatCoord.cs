using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct FloatCoord
    {
        [MarshalAs(UnmanagedType.R4)]
        public float left;
        [MarshalAs(UnmanagedType.R4)]
        public float top;
        [MarshalAs(UnmanagedType.R4)]
        public float width;
        [MarshalAs(UnmanagedType.R4)]
        public float height;

        public FloatCoord(float _left, float _top, float _width, float _height)
        {
            left = _left;
            top = _top;
            width = _width;
            height = _height;
        }
    }
}
