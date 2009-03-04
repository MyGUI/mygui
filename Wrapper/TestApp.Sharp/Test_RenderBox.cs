using System;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_RenderBox
    {
        public static void Test()
        {
            RenderBox box = Gui.Instance.CreateWidget<RenderBox>("RenderBox", new IntCoord(320, 420, 100, 100), Align.Default, "Main");

            box.EventUpdateViewport += new RenderBox.HandleUpdateViewport(box_EventUpdateViewport);
        }

        static void box_EventUpdateViewport(RenderBox _sender)
        {
            Export.DebugOut("EventUpdateViewport");
        }
    }
}
