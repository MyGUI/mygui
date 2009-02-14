using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IntCoord
    {
        public int left;
        public int top;
        public int width;
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
