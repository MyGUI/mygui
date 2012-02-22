using System;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    public class Test_Widget
    {
        public static void Test()
        {
            Widget widget = Gui.Instance.CreateWidget<Widget>("Button", new IntCoord(20, 220, 100, 100), Align.Default, "Main");
            Widget child = widget.CreateWidget<Widget>("Button", new IntCoord(10, 10, 26, 26), Align.Default, "name1");

            //widget.EventActionInfo += new Widget.HandleActionInfo(widget_EventActionInfo);
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

            bool flag = widget.IsUserString("key");
            widget.SetUserString("key", "value");
            flag = widget.IsUserString("key");
            string name = widget.GetUserString("key");
            widget.ClearUserString("key");
            flag = widget.IsUserString("key");
            name = widget.GetUserString("key");

            int height = widget.GetHeight();
            int width = widget.GetWidth();
            int rigth = widget.GetRight();
            int bottom = widget.GetBottom();
            int top = widget.GetTop();
            top = widget.GetAbsoluteTop();
            int left = widget.GetLeft();
            left = widget.GetAbsoluteLeft();
            IntCoord coord = widget.GetAbsoluteCoord();
            IntRect rect = widget.GetAbsoluteRect();
            IntPoint point = widget.GetAbsolutePosition();

            Align align = widget.Align;
            widget.Align = Align.Center;

            //widget.EnableToolTip = !widget.EnableToolTip;
            widget.NeedToolTip = !widget.NeedToolTip;

            Widget client = widget.GetClientWidget();
            coord = widget.GetClientCoord();

            string pointer = widget.Pointer;
            widget.Pointer = "Beam";

            widget.SetEnabledSilent(true);
            widget.Enabled = !widget.Enabled;
            widget.Enabled = !widget.Enabled;

            //widget.SetMaskPick("");
            widget.InheritsPick = !widget.InheritsPick;

            widget.NeedMouseFocus = !widget.NeedMouseFocus;
            widget.NeedMouseFocus = !widget.NeedMouseFocus;

            widget.NeedKeyFocus = !widget.NeedKeyFocus;
            widget.NeedKeyFocus = !widget.NeedKeyFocus;

            Widget find = widget.FindWidget("name1");
            uint count = widget.GetChildCount();
            Widget childat = widget.GetChildAt(0);
            Widget parent = widget.GetParent();
            parent = childat.GetParent();

            flag = widget.IsRootWidget();
            flag = childat.IsRootWidget();

            //childat.SetState("disabled");

            childat.InheritsAlpha = !childat.InheritsAlpha;
            widget.Alpha = 0.5f;

            //widget.Caption = "Widget";

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

            name = widget.GetName();
        }

        static void widget_EventMouseLostFocus(Widget _sender, Widget _new)
        {
            Export.DebugOut("EventMouseLostFocus  _new=" + (_new == null ? "null" : _new.ToString()));
        }

        static void widget_EventMouseSetFocus(Widget _sender, Widget _old)
        {
            Export.DebugOut("EventMouseSetFocus  _old=" + (_old == null ? "null" : _old.ToString()));
        }

        static void widget_EventMouseDrag(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Export.DebugOut("EventMouseDrag  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseMove(Widget _sender, int _left, int _top)
        {
            Export.DebugOut("EventMouseMove  _left=" + _left.ToString() + "   _top=" + _top.ToString());
        }

        static void widget_EventMouseWheel(Widget _sender, int _rel)
        {
            Export.DebugOut("EventMouseWheel  _rel=" + _rel.ToString());
        }

        static void widget_EventMouseButtonReleased(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Export.DebugOut("EventMouseMove  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseButtonPressed(Widget _sender, int _left, int _top, MouseButton _id)
        {
            Export.DebugOut("EventMouseButtonPressed  _left=" + _left.ToString() + "   _top=" + _top.ToString() + "   _id=" + _id.ToString());
        }

        static void widget_EventMouseButtonClick(Widget _sender)
        {
            Export.DebugOut("EventMouseButtonClick");
        }

        static void widget_EventMouseButtonDoubleClick(Widget _sender)
        {
            Export.DebugOut("EventMouseButtonDoubleClick");
        }

        static void widget_EventKeyLostFocus(Widget _sender, Widget _new)
        {
            Export.DebugOut("EventKeyLostFocus  _new=" + (_new == null ? "null" : _new.ToString()));
        }

        static void widget_EventKeySetFocus(Widget _sender, Widget _old)
        {
            Export.DebugOut("EventKeySetFocus  _old=" + (_old == null ? "null" : _old.ToString()));
        }

        static void widget_EventKeyButtonReleased(Widget _sender, KeyCode _key)
        {
            Export.DebugOut("EventKeyButtonReleased  _key=" + _key.ToString());
        }

        static void widget_EventKeyButtonPressed(Widget _sender, KeyCode _key, uint _char)
        {
            Export.DebugOut("EventMouseMove  _key=" + _key.ToString() + "   _char=" + _char.ToString());
        }

        static void widget_EventRootMouseChangeFocus(Widget _sender, bool _focus)
        {
            Export.DebugOut("EventRootMouseChangeFocus  _focus=" + _focus.ToString());
        }

        static void widget_EventRootKeyChangeFocus(Widget _sender, bool _focus)
        {
            Export.DebugOut("EventRootKeyChangeFocus  _focus=" + _focus.ToString());
        }

        static void widget_EventToolTip(Widget _sender, ref ToolTipInfo _info)
        {
            Export.DebugOut("EventToolTip  _info=" + _info.ToString());
        }

        /*static void widget_EventActionInfo(Widget _sender, string _key, string _value)
        {
            Export.DebugOut("EventActionInfo  _key=" + _key + "  _value=" + _value);
        }*/
    }
}
