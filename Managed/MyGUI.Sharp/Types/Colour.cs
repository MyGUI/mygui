using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct Colour
    {
        public int red;
        public int green;
        public int blue;
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
