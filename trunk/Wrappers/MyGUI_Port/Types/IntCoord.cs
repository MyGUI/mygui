using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IntCoord
    {
        [MarshalAs(UnmanagedType.I4)]
        public int left;
        [MarshalAs(UnmanagedType.I4)]
        public int top;
        [MarshalAs(UnmanagedType.I4)]
        public int width;
        [MarshalAs(UnmanagedType.I4)]
        public int height;

        public IntCoord(int _left, int _top, int _width, int _height)
        {
            left = _left;
            top = _top;
            width = _width;
            height = _height;
        }
    }
}
