using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DDWidgetState
    {
        [MarshalAs(UnmanagedType.U4)]
        public uint index;
        [MarshalAs(UnmanagedType.U1)]
        public bool update;
        [MarshalAs(UnmanagedType.U1)]
        public bool accept;
        [MarshalAs(UnmanagedType.U1)]
        public bool refuse;
    }
}
