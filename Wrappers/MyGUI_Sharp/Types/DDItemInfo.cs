using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DDItemInfo
    {
		public IntPtr sender;
        [MarshalAs(UnmanagedType.U4)]
        public uint sender_index;
        public IntPtr receiver;
        [MarshalAs(UnmanagedType.U4)]
        public uint receiver_index;
    }
}
