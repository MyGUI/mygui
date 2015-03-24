using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct ToolTipInfo
    {
        [MarshalAs(UnmanagedType.U1)]
        public bool visible;
        [MarshalAs(UnmanagedType.U4)]
        public uint index;
        [MarshalAs(UnmanagedType.I4)]
        public int x;
        [MarshalAs(UnmanagedType.I4)]
        public int y;
    }
}
