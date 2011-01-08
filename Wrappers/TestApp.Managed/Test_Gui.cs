using System;
using MyGUI.Managed.Demo;
using MyGUI.Managed;

namespace TestApp.Sharp
{
    public class Test_Gui
    {
        public static void Test()
        {
            Button button = Gui.Instance.CreateWidget<Button>("Button", new IntCoord(760, 420, 100, 100), Align.Default, "Main");
            button.Caption = "Gui";

            bool focus = InputManager.Instance.KeyFocus;
            Widget widget = InputManager.Instance.KeyFocusWidget;
            InputManager.Instance.KeyFocusWidget = button;
            InputManager.Instance.ResetKeyFocus();

            InputManager.Instance.AddWidgetModal(button);
            InputManager.Instance.RemoveWidgetModal(button);

            LayerManager.Instance.AttachToLayer("Popup", button);
            LayerManager.Instance.UpWidget(button);

            Gui.Instance.LoadResource("core_layer.xml");

            focus = InputManager.Instance.MouseFocus;
            widget = InputManager.Instance.MouseFocusWidget;

            Gui.Instance.LoadLayout("Mix.layout");
        }

        public static void Update()
        {
            string mouse = InputManager.Instance.MouseFocus.ToString() + "(" + (InputManager.Instance.MouseFocusWidget == null ? "null" : InputManager.Instance.MouseFocusWidget.ToString()) + ")";
            string key = InputManager.Instance.KeyFocus.ToString() + "(" + (InputManager.Instance.KeyFocusWidget == null ? "null" : InputManager.Instance.KeyFocusWidget.ToString()) + ")";
            Export.DebugOut("GUI: mouse=" + mouse + "   key=" + key);
        }
    }
}
