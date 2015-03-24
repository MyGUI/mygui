using System;
using MyGUI.Managed;

namespace TestApp.Managed
{
    public class Test_Widget
    {
        public static void Test()
        {
            Widget widget = Gui.Instance.CreateWidget<Widget>("Button", new IntCoord(20, 220, 100, 100), Align.Default, "Main");

            widget.EventToolTip += new Widget.HandleToolTip(widget_EventToolTip);
            widget.EventRootKeyChangeFocus += new Widget.HandleRootKeyChangeFocus(widget_EventRootKeyChangeFocus);
            widget.EventRootMouseChangeFocus += new Widget.HandleRootMouseChangeFocus(widget_EventRootMouseChangeFocus);
            widget.EventKeyButtonPressed += new Widget.HandleKeyButtonPressed(widget_EventKeyButtonPressed);
            widget.EventKeyButtonReleased += new Widget.HandleKeyButtonReleased(widget_EventKeyButtonReleased);
            widget.EventKeySetFocus += new Widget.HandleKeySetFocus(widget_EventKeySetFocus);
            widget.EventKeyLostFocus += new Widget.HandleKeyLostFocus(widget_EventKeyLostFocus);
            widget.EventMouseButtonDoubleClick += new Widget.HandleMouseButtonDoubleClick(widget_EventMouseButtonDoubleClick);
            widget.EventMouseButtonClick += new Widget.HandleMouseButtonClick(widget_EventMouseButtonClick);
            widget.EventMouseButtonPressed += new Widget.HandleMouseButtonPressed(widget_EventMouseButtonPressed);
            widget.EventMouseButtonReleased += new Widget.HandleMouseButtonReleased(widget_EventMouseButtonReleased);
            widget.EventMouseWheel += new Widget.HandleMouseWheel(widget_EventMouseWheel);
            widget.EventMouseMove += new Widget.HandleMouseMove(widget_EventMouseMove);
            widget.EventMouseDrag += new Widget.HandleMouseDrag(widget_EventMouseDrag);
            widget.EventMouseSetFocus += new Widget.HandleMouseSetFocus(widget_EventMouseSetFocus);
            widget.EventMouseLostFocus += new Widget.HandleMouseLostFocus(widget_EventMouseLostFocus);

			Widget child = widget.CreateWidget<Widget>("Button", new IntCoord(10, 10, 26, 26), Align.Default, "name1");

			bool flag = widget.IsUserString("key");
            widget.SetUserString("key", "value");
            flag = widget.IsUserString("key");
            string name = widget.GetUserString("key");
            widget.ClearUserString("key");
            flag = widget.IsUserString("key");
            name = widget.GetUserString("key");

            int height = widget.Height;
            int width = widget.Width;
            int rigth = widget.Right;
            int bottom = widget.Bottom;
            int top = widget.Top;
            top = widget.AbsoluteTop;
            int left = widget.Left;
            left = widget.AbsoluteLeft;
            IntCoord coord = widget.AbsoluteCoord;
            IntRect rect = widget.AbsoluteRect;
            IntPoint point = widget.AbsolutePosition;

            Align align = widget.Align;
            widget.Align = Align.Center;

            //widget.EnableToolTip = !widget.EnableToolTip;
            widget.NeedToolTip = !widget.NeedToolTip;

            Widget client = widget.ClientWidget;
            coord = widget.ClientCoord;

            string pointer = widget.Pointer;
            widget.Pointer = "Beam";

            widget.SetEnabledSilent(true);
            widget.Enabled = !widget.Enabled;
            widget.Enabled = !widget.Enabled;

            widget.InheritsPick = !widget.InheritsPick;

            widget.NeedMouseFocus = !widget.NeedMouseFocus;
            widget.NeedMouseFocus = !widget.NeedMouseFocus;

            widget.NeedKeyFocus = !widget.NeedKeyFocus;
            widget.NeedKeyFocus = !widget.NeedKeyFocus;

            Widget find = widget.FindWidget("name1");
            uint count = widget.ChildCount;
            Widget childat = widget.GetChildAt(0);
            Widget parent = widget.Parent;
            parent = childat.Parent;

            flag = widget.IsRootWidget;
            flag = childat.IsRootWidget;

            childat.InheritsAlpha = !childat.InheritsAlpha;
            widget.Alpha = 0.5f;

            widget.Visible = !widget.Visible;
            widget.Visible = !widget.Visible;

            widget.SetRealCoord(0, 0, 0.1f, 0.1f);
            widget.SetRealSize(0.2f, 0.2f);
            widget.SetRealPosition(0.1f, 0.1f);

            widget.SetPosition(10, 10);
            widget.SetSize(10, 10);
            widget.SetCoord(21, 221, 101, 101);

            widget.Position = new IntPoint(11, 11);
            widget.Size = new IntSize(12, 12);
            widget.Coord = new IntCoord(20, 220, 100, 100);

            name = widget.Name;
        }

        static void widget_EventMouseLostFocus(Widget _sender, Widget _new)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseLostFocus  _new=" + (_new == null ? "null" : _new.ToString()));
        }

        static void widget_EventMouseSetFocus(Widget _sender, Widget _old)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseSetFocus  _old=" + (_old == null ? "null" : _old.ToString()));
        }

        static void widget_EventMouseDrag(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseDrag  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseMove(Widget _sender, int _left, int _top)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseMove  _left=" + _left.ToString() + "   _top=" + _top.ToString());
        }

        static void widget_EventMouseWheel(Widget _sender, int _rel)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseWheel  _rel=" + _rel.ToString());
        }

        static void widget_EventMouseButtonReleased(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseMove  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseButtonPressed(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseButtonPressed  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseButtonClick(Widget _sender)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseButtonClick");
        }

        static void widget_EventMouseButtonDoubleClick(Widget _sender)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseButtonDoubleClick");
        }

        static void widget_EventKeyLostFocus(Widget _sender, Widget _new)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventKeyLostFocus  _new=" + (_new == null ? "null" : _new.ToString()));
        }

        static void widget_EventKeySetFocus(Widget _sender, Widget _old)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventKeySetFocus  _old=" + (_old == null ? "null" : _old.ToString()));
        }

        static void widget_EventKeyButtonReleased(Widget _sender, KeyCode _key)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventKeyButtonReleased  _key=" + _key.ToString());
        }

        static void widget_EventKeyButtonPressed(Widget _sender, KeyCode _key, uint _char)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseMove  _key=" + _key.ToString() + "   _char=" + _char.ToString());
        }

        static void widget_EventRootMouseChangeFocus(Widget _sender, bool _focus)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventRootMouseChangeFocus  _focus=" + _focus.ToString());
        }

        static void widget_EventRootKeyChangeFocus(Widget _sender, bool _focus)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventRootKeyChangeFocus  _focus=" + _focus.ToString());
        }

        static void widget_EventToolTip(Widget _sender, ToolTipInfo _info)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventToolTip  _info=" + _info.ToString());
        }
    }
}
