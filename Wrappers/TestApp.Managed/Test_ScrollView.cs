using System;
using MyGUI.Managed;

namespace TestApp.Sharp
{
    public class Test_ScrollView
    {
        public static void Test()
        {
            ScrollView view = Gui.Instance.CreateWidget<ScrollView>("ScrollView", new IntCoord(420, 120, 100, 100), Align.Default, "Main");
            Widget child = view.CreateWidget<Widget>("Button", new IntCoord(10, 10, 26, 26), Align.Default);

            view.SetCanvasSize(210, 210);
            view.CanvasSize = new IntSize(211, 211);
            IntSize size = view.CanvasSize;

            view.CanvasAlign = Align.Center;

            view.VisibleHScroll = !view.VisibleHScroll;
            view.VisibleVScroll = !view.VisibleVScroll;

        }
    }
}
