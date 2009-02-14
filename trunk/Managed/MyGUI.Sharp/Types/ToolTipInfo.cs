using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ToolTipInfo
    {
        public bool visible;
        public uint index;
        public int x;
        public int y;
    }
}
