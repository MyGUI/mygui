using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
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
