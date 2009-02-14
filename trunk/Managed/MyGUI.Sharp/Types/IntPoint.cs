using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IntPoint
    {
        public int left;
        public int top;

        public IntPoint(int _left, int _top)
        {
            left = _left;
            top = _top;
        }
    }
}
