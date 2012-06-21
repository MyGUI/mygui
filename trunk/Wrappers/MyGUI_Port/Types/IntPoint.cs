using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
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
