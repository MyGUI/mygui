using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
    public struct IntRect
    {
        public int left;
        public int top;
        public int right;
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
