using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct FloatRect
    {
        [MarshalAs(UnmanagedType.R4)]
        public float left;
        [MarshalAs(UnmanagedType.R4)]
        public float top;
        [MarshalAs(UnmanagedType.R4)]
        public float right;
        [MarshalAs(UnmanagedType.R4)]
        public float bottom;

        public FloatRect(float _left, float _top, float _right, float _bottom)
        {
            left = _left;
            top = _top;
            right = _right;
            bottom = _bottom;
        }
    }
}
