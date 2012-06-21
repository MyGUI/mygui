using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
    public struct FloatCoord
    {
        public float left;
        public float top;
        public float width;
        public float height;

        public FloatCoord(float _left, float _top, float _width, float _height)
        {
            left = _left;
            top = _top;
            width = _width;
            height = _height;
        }
    }
}
