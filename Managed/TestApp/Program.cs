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
            Gui.Instance.CreateWidget<Edit>("Edit", new IntCoord(10, 400, 100, 100), Align.Default, "Overlapped");
            Widget widget = Gui.Instance.CreateWidget<Widget>("Panel", new IntCoord(10, 10, 200, 200), Align.Default, "Overlapped");
            List list = widget.CreateWidget<List>("List", new IntCoord(10, 10, 100, 100), Align.Left | Align.Top);
            list.InsertItemAt(0, "line1", null);
            list.AddItem("line2", null);

            /*widget.KeyButtonPressed += new Widget.HandleKeyButtonPressed(widget_KeyButtonPressed);
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
            widget.RootMouseChangeFocus += new Widget.HandleRootMouseChangeFocus(widget_RootMouseChangeFocus);*/

            list.ListSelectAccept += new List.HandleListSelectAccept(widget_ListSelectAccept);
            list.ListSelectAccept += new List.HandleListSelectAccept(widget_ListSelectAccept2);
            list.ListMouseItemActivate += new List.HandleListMouseItemActivate(widget_ListMouseItemActivate);
            list.ListChangePosition += new List.HandleListChangePosition(widget_ListChangePosition);

            Button button = Gui.Instance.CreateWidget<Button>("Panel", new IntCoord(410, 10, 200, 200), Align.Default, "Overlapped");
            List wid = button.CreateWidget<List>("List", new IntCoord(20, 20, 100, 100), Align.Default);
            wid.AddItem("test", null);

            Button test1;
            ComboBox test2;
            DDContainer test3;
            Edit test4;
            HScroll test5;
            ItemBox test6;
            List test7;
            MenuBar test8;
            MenuCtrl test9;
            Message test10;
            MultiList test11;
            PopupMenu test12;
            Progress test13;
            RenderBox test14;
            ScrollView test15;
            StaticImage test16;
            StaticText test17;
            Tab test18;
            VScroll test19;
            Widget test20;
            Window test21;

            MenuItem test22;
            TabItem test23;

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
            string name = (_value1 as List).GetItemNameAt(0);

            if (_value2 == 1)
                demo.Export.Shutdown();
            //demo.Export.Shutdown();
            //throw new Exception("The method or operation is not implemented.");
        }

        static void widget_ListSelectAccept2(Widget _value1, uint _value2)
        {
            string name = (_value1 as List).GetItemNameAt(0);

            if (_value2 == 1)
                demo.Export.Shutdown();
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
