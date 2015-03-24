using MyGUI.Sharp;

namespace TestApp.Sharp
{
	public class Test_ScrollBar
	{
		public static void Test()
		{
			ScrollBar scroll = Gui.Instance.CreateWidget<ScrollBar>("ScrollBarV", new IntCoord(580, 120, 16, 100), Align.Default, "Main");
			scroll.EventScrollChangePosition += new ScrollBar.HandleScrollChangePosition(scroll_EventScrollChangePosition);

			scroll.MinTrackSize = scroll.MinTrackSize + 1;
			scroll.TrackSize = scroll.TrackSize + 1;

			int size = scroll.LineSize;
			scroll.ScrollViewPage = scroll.ScrollViewPage + 1;
			scroll.ScrollPage = scroll.ScrollPage + 1;
			scroll.ScrollRange = scroll.ScrollRange + 10;
			scroll.ScrollPosition = scroll.ScrollPosition + 1;
		}

		static void scroll_EventScrollChangePosition(ScrollBar _sender, uint _position)
		{
			Gui.Instance.Log("TestApp", LogLevel.Info, "EventScrollChangePosition  position=" + _position.ToString());
		}
	}
}
