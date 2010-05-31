using System;
using MyGUI.Managed;

namespace TestApp.Sharp
{
    public class Test_StaticImage
    {
        public static void Test()
        {
            StaticImage image = Gui.Instance.CreateWidget<StaticImage>("StaticImage", new IntCoord(320, 120, 100, 100), Align.Default, "Main");
            image.SetItemResource("pic_CoreMessageIcon");
            image.SetItemGroup("Icons");
            image.SetItemName("Warning");

            image = Gui.Instance.CreateWidget<StaticImage>("StaticImage", new IntCoord(320, 20, 100, 100), Align.Default, "Main");
            image.SetImageTexture("core.png");
            image.SetImageCoord(new IntCoord(0, 0, 1024, 256));
            image.SetImageTile(new IntSize(100, 100));
            image.ImageIndex = 1;
        }
    }
}
