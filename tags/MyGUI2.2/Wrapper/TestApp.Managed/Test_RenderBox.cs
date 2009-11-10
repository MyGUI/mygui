using System;
using MyGUI.Managed;
using MyGUI.Managed.Extension;


namespace TestApp.Managed
{
    public class Test_RenderBox
    {
        static RenderBoxWrapper mWrapper;

        public static void Test()
        {
            RenderBox box = Gui.Instance.CreateWidget<RenderBox>("RenderBox", new IntCoord(320, 420, 100, 100), Align.Default, "Main");
            box.EventUpdateViewport += new RenderBox.HandleUpdateViewport(box_EventUpdateViewport);

            mWrapper = new RenderBoxWrapper(box.GetNativeIntPtr());
            mWrapper.InjectObject("axes.mesh");
            mWrapper.AutoRotation = true;
            mWrapper.MouseRotation = true;

        }

        static void box_EventUpdateViewport(RenderBox _sender)
        {
            ExampleApplication.DebugOut("EventUpdateViewport");
        }
    }
}
