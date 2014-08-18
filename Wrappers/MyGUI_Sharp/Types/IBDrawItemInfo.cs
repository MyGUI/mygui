using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IBDrawItemInfo
    {
        [MarshalAs(UnmanagedType.U4)]
        public uint index;
        [MarshalAs(UnmanagedType.U1)]
        public bool update;
        [MarshalAs(UnmanagedType.U1)]
        public bool select;
        [MarshalAs(UnmanagedType.U1)]
        public bool active;
        [MarshalAs(UnmanagedType.U1)]
        public bool drag;
        [MarshalAs(UnmanagedType.U1)]
        public bool drop_accept;
        [MarshalAs(UnmanagedType.U1)]
        public bool drop_refuse;
    }
}
