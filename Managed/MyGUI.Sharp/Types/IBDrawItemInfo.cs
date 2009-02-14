using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IBDrawItemInfo
    {
        public uint index;
        public bool update;
        public bool select;
        public bool active;
        public bool drag;
        public bool drop_accept;
        public bool drop_refuse;
    }
}
