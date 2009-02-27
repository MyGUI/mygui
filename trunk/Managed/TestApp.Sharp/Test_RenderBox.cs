using System;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_RenderBox
    {
        public static void Test()
        {
            RenderBox box = new RenderBox(IntPtr.Zero, WidgetStyle.Overlapped, "RenderBox", new IntCoord(320, 420, 100, 100), Align.Default, "Overlapped", "");

            box.EventUpdateViewport += new RenderBox.HandleUpdateViewport(box_EventUpdateViewport);
        }

        static void box_EventUpdateViewport(RenderBox _sender)
        {
            Export.DebugOut("EventUpdateViewport");
        }
    }
}
