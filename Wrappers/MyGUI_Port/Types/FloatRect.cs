using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
    public struct FloatRect
    {
        public float left;
        public float top;
        public float right;
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
