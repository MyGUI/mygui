using System;
using MyGUI.Sharp;

namespace TestApp.Sharp
{
    public class Test_Button
    {
        public static void Test()
        {
            Button button = Gui.Instance.CreateWidget<Button>("MultiListButton", new IntCoord(10, 10, 100, 100), Align.Default, "Main");

            uint index = uint.MaxValue;
            index = button.ImageIndex;
            button.ImageIndex = uint.MaxValue;
            index = button.ImageIndex;
            button.ImageIndex = 0;
            index = button.ImageIndex;

            bool value = false;
            value = button.StateCheck;
            button.StateCheck = false;
            value = button.StateCheck;
            button.StateCheck = true;
            value = button.StateCheck;

            value = false;
            value = button.ButtonPressed;
            button.ButtonPressed = false;
            value = button.ButtonPressed;
            button.ButtonPressed = true;
            value = button.ButtonPressed;

            StaticImage image = button.GetStaticImage();
        }
    }
}
