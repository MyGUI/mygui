using System;
using System.Runtime.InteropServices;

namespace MyGUI
{
    public struct IBNotifyItemData
    {
        public uint index;
        public IBNotifyItemData.NotifyItem notify;
        public int x;
        public int y;
        public MouseButton id;
        public KeyCode code;
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
