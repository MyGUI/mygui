using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IntSize
    {
        public int width;
        public int height;

        public IntSize(int _width, int _height)
        {
            width = _width;
            height = _height;
        }
    }
}
