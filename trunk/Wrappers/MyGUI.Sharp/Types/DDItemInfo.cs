using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DDItemInfo
    {
        [MarshalAs(UnmanagedType.Interface)]
        public DDContainer sender;
        [MarshalAs(UnmanagedType.U4)]
        public uint sender_index;
        [MarshalAs(UnmanagedType.Interface)]
        public DDContainer receiver;
        [MarshalAs(UnmanagedType.U4)]
        public uint receiver_index;
    }
}
