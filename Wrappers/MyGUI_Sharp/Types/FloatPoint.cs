using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct FloatPoint
    {
        [MarshalAs(UnmanagedType.R4)]
        public float left;
        [MarshalAs(UnmanagedType.R4)]
        public float top;

        public FloatPoint(float _left, float _top)
        {
            left = _left;
            top = _top;
        }
    }
}
