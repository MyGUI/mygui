using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IBNotifyItemData
    {
        [MarshalAs(UnmanagedType.U4)]
        public uint index;
        [MarshalAs(UnmanagedType.I4)]
        public IBNotifyItemData.NotifyItem notify;
        [MarshalAs(UnmanagedType.I4)]
        public int x;
        [MarshalAs(UnmanagedType.I4)]
        public int y;
        [MarshalAs(UnmanagedType.I4)]
        public MouseButton id;
        [MarshalAs(UnmanagedType.I4)]
        public KeyCode code;
        [MarshalAs(UnmanagedType.U4)]
        public uint key;

        public enum NotifyItem : int
        {
            MousePressed = 0,
            MouseReleased = 1,
            KeyPressed = 2,
            KeyReleased = 3,
        }
    }
}
