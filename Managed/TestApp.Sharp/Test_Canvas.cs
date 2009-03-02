using System;
using MyGUI.Sharp;

namespace TestApp.Sharp
{
    public class Test_Canvas
    {
        public static void Test()
        {
            Canvas canvas = Gui.Instance.CreateWidget<Canvas>("Canvas", new IntCoord(20, 20, 100, 100), Align.Default, "Main");
        }
    }
}
