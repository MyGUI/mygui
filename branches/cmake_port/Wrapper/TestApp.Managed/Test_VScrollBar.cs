using System;
using MyGUI.Managed;


namespace TestApp.Managed
{
    public class Test_VScrollBar
    {
        public static void Test()
        {
            VScrollBar scroll = Gui.Instance.CreateWidget<VScrollBar>("VScroll", new IntCoord(580, 120, 16, 100), Align.Default, "Main");
            scroll.EventScrollChangePosition += new VScrollBar.HandleScrollChangePosition(scroll_EventScrollChangePosition);

            scroll.MinTrackSize = scroll.MinTrackSize + 1;
            scroll.TrackSize = scroll.TrackSize + 1;

            int size = scroll.GetLineSize();
            scroll.ScrollViewPage = scroll.ScrollViewPage + 1;
            scroll.ScrollPage = scroll.ScrollPage + 1;
            scroll.ScrollRange = scroll.ScrollRange + 10;
            scroll.ScrollPosition = scroll.ScrollPosition + 1;
        }

        static void scroll_EventScrollChangePosition(VScrollBar _sender, uint _position)
        {
            ExampleApplication.DebugOut("EventScrollChangePosition  position=" + _position.ToString());
        }
    }
}
