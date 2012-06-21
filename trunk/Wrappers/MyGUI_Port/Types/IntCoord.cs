using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
    public struct IntCoord
    {
        public int left;
        public int top;
        public int width;
        public int height;

		public int right()
		{
			return left + width;
		}

		public int bottom()
		{
			return top + height;
		}

		public IntPoint point()
		{
			return new IntPoint(left, top);
		}

		public IntSize size()
		{
			return new IntSize(width, height);
		}

		public IntCoord(int _left, int _top, int _width, int _height)
        {
            left = _left;
            top = _top;
            width = _width;
            height = _height;
        }
    }
}
