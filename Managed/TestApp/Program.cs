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
            Button button = Gui.Instance.CreateWidget<Button>("Button", new IntCoord(10, 10, 200, 26), Align.Default, "Main");
            button.Caption = "Button";

            ComboBox combo = Gui.Instance.CreateWidget<ComboBox>("ComboBox", new IntCoord(10, 50, 200, 26), Align.Default, "Main");
            combo.Caption = "ComboBox";
            combo.AddItem("line1", null);
            combo.AddItem("line2", null);
            combo.AddItem("line3", null);

            Edit edit = Gui.Instance.CreateWidget<Edit>("Edit", new IntCoord(10, 90, 200, 26), Align.Default, "Main");
            edit.Caption = "Edit";

            List list = Gui.Instance.CreateWidget<List>("List", new IntCoord(10, 130, 200, 126), Align.Default, "Main");
            list.Caption = "List";
            list.AddItem("line1", null);
            list.AddItem("line2", null);
            list.AddItem("line3", null);

            Widget widget = Gui.Instance.CreateWidget<Widget>("Panel", new IntCoord(10, 260, 200, 126), Align.Default, "Main");

            /*Window window = Gui.Instance.CreateWidget<Window>("WindowCS", new IntCoord(10, 10, 200, 200), Align.Default, "Overlapped");
            List list = window.CreateWidget<List>("List", new IntCoord(10, 10, 100, 100), Align.Left | Align.Top);
            list.InsertItemAt(0, "line1", null);
            list.AddItem("line2", null);

            list.ListSelectAccept += new List.HandleListSelectAccept(widget_ListSelectAccept);
            list.ListSelectAccept += new List.HandleListSelectAccept(widget_ListSelectAccept2);
            list.ListMouseItemActivate += new List.HandleListMouseItemActivate(widget_ListMouseItemActivate);
            list.ListChangePosition += new List.HandleListChangePosition(widget_ListChangePosition);

            Widget client = window.GetClientWidget();
            //client.SetVisible(false);

            //client.Dispose();
            //client = null;
            window.Dispose();
            window = null;

            Button button = Gui.Instance.CreateWidget<Button>("Panel", new IntCoord(410, 10, 200, 200), Align.Default, "Overlapped");
            List wid = button.CreateWidget<List>("List", new IntCoord(20, 20, 100, 100), Align.Default);
            wid.AddItem("test", null);
            */
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
            TabItem test23;//*/

        }

        static void FrameStart(float _time)
        {
        }

    }
}
