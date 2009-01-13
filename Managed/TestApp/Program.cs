using System;
using System.Collections.Generic;
using System.Text;
using MMyGUI;

namespace TestApp
{
    class Program
    {

        static void Main(string[] args)
        {
            demo.Export.Initialise();

            Initialise();

            demo.Export.AddFrameDelegate(new demo.Types.HandleFrameStart(FrameStart));
            demo.Export.Run();
        }

        static void Initialise()
        {
            List widget = new List("List", new IntCoord(10, 10, 100, 100), Align.Left | Align.Top, "Overlapped");
            widget.insertItemAt(0, "line1", null);
            widget.addItem("line2", null);

            widget.KeyButtonPressed += new Widget.HandleKeyButtonPressed(widget_KeyButtonPressed);
            widget.KeyButtonReleased += new Widget.HandleKeyButtonReleased(widget_KeyButtonReleased);
            widget.KeyLostFocus += new Widget.HandleKeyLostFocus(widget_KeyLostFocus);
            widget.KeySetFocus += new Widget.HandleKeySetFocus(widget_KeySetFocus);
            widget.MouseButtonClick +=new Widget.HandleMouseButtonClick(widget_MouseButtonClick);
            widget.MouseButtonDoubleClick += new Widget.HandleMouseButtonDoubleClick(widget_MouseButtonDoubleClick);
            widget.MouseButtonPressed += new Widget.HandleMouseButtonPressed(widget_MouseButtonPressed);
            widget.MouseButtonReleased += new Widget.HandleMouseButtonReleased(widget_MouseButtonReleased);
            widget.MouseDrag += new Widget.HandleMouseDrag(widget_MouseDrag);
            widget.MouseLostFocus += new Widget.HandleMouseLostFocus(widget_MouseLostFocus);
            widget.MouseMove += new Widget.HandleMouseMove(widget_MouseMove);
            widget.MouseSetFocus += new Widget.HandleMouseSetFocus(widget_MouseSetFocus);
            widget.MouseWheel += new Widget.HandleMouseWheel(widget_MouseWheel);
            widget.RootKeyChangeFocus += new Widget.HandleRootKeyChangeFocus(widget_RootKeyChangeFocus);
            widget.RootMouseChangeFocus += new Widget.HandleRootMouseChangeFocus(widget_RootMouseChangeFocus);

            widget.ListSelectAccept += new List.HandleListSelectAccept(widget_ListSelectAccept);
            widget.ListMouseItemActivate += new List.HandleListMouseItemActivate(widget_ListMouseItemActivate);
            widget.ListChangePosition += new List.HandleListChangePosition(widget_ListChangePosition);
            //Button
        }

        static void widget_RootMouseChangeFocus(Widget _value1, bool _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_RootKeyChangeFocus(Widget _value1, bool _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseWheel(Widget _value1, int _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseSetFocus(Widget _value1, Widget _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseMove(Widget _value1, int _value2, int _value3)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseLostFocus(Widget _value1, Widget _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseDrag(Widget _value1, int _value2, int _value3)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseButtonReleased(Widget _value1, int _value2, int _value3, MouseButton _value4)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseButtonPressed(Widget _value1, int _value2, int _value3, MouseButton _value4)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseButtonDoubleClick(Widget _value1)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_KeySetFocus(Widget _value1, Widget _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_KeyLostFocus(Widget _value1, Widget _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_KeyButtonReleased(Widget _value1, KeyCode _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_KeyButtonPressed(Widget _value1, KeyCode _value2, uint _value3)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_ListChangePosition(Widget _value1, uint _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_ListMouseItemActivate(Widget _value1, uint _value2)
        {
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_ListSelectAccept(Widget _value1, uint _value2)
        {
            string name = (_value1 as List).getItemNameAt(0);
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_MouseButtonClick(Widget _value1)
        {
            _value1.Dispose();
            demo.Export.Shutdown();
        }

        static void FrameStart(float _time)
        {
        }

    }
}
