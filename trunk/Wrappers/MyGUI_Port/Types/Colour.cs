using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Colour
    {
        [MarshalAs(UnmanagedType.R4)]
        public float red;
        [MarshalAs(UnmanagedType.R4)]
        public float green;
        [MarshalAs(UnmanagedType.R4)]
        public float blue;
        [MarshalAs(UnmanagedType.R4)]
        public float alpha;

        public Colour(float _red, float _green, float _blue, float _alpha)
        {
            red = _red;
            green = _green;
            blue = _blue;
            alpha = _alpha;
        }

        public Colour(float _red, float _green, float _blue)
        {
            red = _red;
            green = _green;
            blue = _blue;
            alpha = 1;
        }

        public readonly static Colour Zero = new Colour(0, 0, 0, 0);
        public readonly static Colour White = new Colour(1, 1, 1);
        public readonly static Colour Black = new Colour(0, 0, 0);
        public readonly static Colour Red = new Colour(1, 0, 0);
        public readonly static Colour Green = new Colour(0, 1, 0);
        public readonly static Colour Blue = new Colour(0, 0, 1);
    }
}
