using MyGUI.Sharp;

namespace TestApp.Sharp
{
	public class Test_Window
	{
		public static void Test()
		{
			Window window = Gui.Instance.CreateWidget<Window>("WindowCSX", new IntCoord(620, 620, 100, 100), Align.Default, "Popup");
			window.Caption = "WindowCSX";

			window.EventWindowChangeCoord += new Window.HandleWindowChangeCoord(window_EventWindowChangeCoord);
			window.EventWindowButtonPressed += new Window.HandleWindowButtonPressed(window_EventWindowButtonPressed);

			window.Snap = !window.Snap;
			window.SetMaxSize(1000, 1000);
			window.SetMinSize(80, 80);

			IntSize size = window.MaxSize;
			window.MaxSize = new IntSize(1001, 1001);

			size = window.MinSize;
			window.MinSize = new IntSize(82, 82);

			window.AutoAlpha = !window.AutoAlpha;

			window.Visible = false;
			window.SetVisibleSmooth(true);
		}

		static void window_EventWindowButtonPressed(Window _sender, string _name)
		{
		}

		static void window_EventWindowChangeCoord(Window _sender)
		{
		}
	}
}
