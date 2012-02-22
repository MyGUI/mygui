using System;
using MyGUI.Sharp;

namespace TestApp.Sharp
{
    public class Test_TextBox
    {
        public static void Test()
        {
            TextBox text = Gui.Instance.CreateWidget<TextBox>("TextBox", new IntCoord(320, 220, 100, 100), Align.Default, "Main");
            text.Caption = "TextBox";

            Colour colour = text.TextColour;
            text.TextColour = Colour.White;

            Align align = text.TextAlign;
            text.TextAlign = Align.Center;

            text.FontHeight = text.FontHeight + 1;
            string font = text.FontName;
            text.FontName = "Default";

            IntSize size = text.GetTextSize();
            IntCoord coord = text.GetTextRegion();
        }
    }
}
