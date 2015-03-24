using System;
using MyGUI.Managed;

namespace TestApp.Managed
{
    public class Test_Button
    {
        public static void Test()
        {
            Button button = Gui.Instance.CreateWidget<Button>("Button", new IntCoord(10, 10, 100, 100), Align.Default, "Main");

            bool value = false;
            value = button.StateSelected;
            button.StateSelected = false;
            value = button.StateSelected;
            button.StateSelected = true;
            value = button.StateSelected;
        }
    }
}
