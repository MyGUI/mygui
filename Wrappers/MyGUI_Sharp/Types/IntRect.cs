using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IntRect
    {
        [MarshalAs(UnmanagedType.I4)]
        public int left;
        [MarshalAs(UnmanagedType.I4)]
        public int top;
        [MarshalAs(UnmanagedType.I4)]
        public int right;
        [MarshalAs(UnmanagedType.I4)]
        public int bottom;

        public IntRect(int _left, int _top, int _right, int _bottom)
        {
            left = _left;
            top = _top;
            right = _right;
            bottom = _bottom;
        }
    }
}
