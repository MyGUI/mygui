using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    [StructLayout(LayoutKind.Sequential)]
    public struct DDWidgetState
    {
        public uint index;
        public bool update;
        public bool accept;
        public bool refuse;
    }
}
