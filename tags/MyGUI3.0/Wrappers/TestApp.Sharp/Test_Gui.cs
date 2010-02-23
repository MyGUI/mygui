using System;
using MyGUI.Sharp.Demo;
using MyGUI.Sharp;

namespace TestApp.Sharp
{
    public class Test_Gui
    {
        public static void Test()
        {
            Widget button = Gui.Instance.CreateWidget<Widget>("Button", new IntCoord(760, 420, 100, 100), Align.Default, "Main");
            button.Caption = "Gui";

            bool focus = Gui.Instance.KeyFocus;
            Widget widget = Gui.Instance.KeyFocusWidget;
            Gui.Instance.KeyFocusWidget = button;
            Gui.Instance.ResetKeyFocus();

            Gui.Instance.AddWidgetModal(button);
            Gui.Instance.RemoveWidgetModal(button);

            Gui.Instance.AttachToLayer("Popup", button);
            Gui.Instance.UpWidget(button);

            Gui.Instance.LoadResource("core_layer.xml");

            focus = Gui.Instance.MouseFocus;
            widget = Gui.Instance.MouseFocusWidget;

            Gui.Instance.LoadLayout("Mix.layout");
        }

        public static void Update()
        {
            string mouse = Gui.Instance.MouseFocus.ToString() + "(" + (Gui.Instance.MouseFocusWidget == null ? "null" : Gui.Instance.MouseFocusWidget.ToString()) + ")";
            string key = Gui.Instance.KeyFocus.ToString() + "(" + (Gui.Instance.KeyFocusWidget == null ? "null" : Gui.Instance.KeyFocusWidget.ToString()) + ")";
            Export.DebugOut("GUI: mouse=" + mouse + "   key=" + key);
        }
    }
}
