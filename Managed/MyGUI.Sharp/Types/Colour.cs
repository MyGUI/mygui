using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Colour
    {
        [MarshalAs(UnmanagedType.I4)]
        public int red;
        [MarshalAs(UnmanagedType.I4)]
        public int green;
        [MarshalAs(UnmanagedType.I4)]
        public int blue;
        [MarshalAs(UnmanagedType.I4)]
        public int alpha;

        public Colour(int _red, int _green, int _blue, int _alpha)
        {
            red = _red;
            green = _green;
            blue = _blue;
            alpha = _alpha;
        }
    }
}
