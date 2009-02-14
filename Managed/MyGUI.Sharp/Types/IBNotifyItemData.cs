using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct IBNotifyItemData
    {
        public uint index;
        public IBNotifyItemData.NotifyItem notify;
        public int x;
        public int y;
        public MouseButton id;
        public KeyCode code;
        public uint key;

        public enum NotifyItem
        {
            MousePressed = 0,
            MouseReleased = 1,
            KeyPressed = 2,
            KeyReleased = 3,
        }
    }
}
