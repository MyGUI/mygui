using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    class Program
    {

        static void Main(string[] args)
        {
            Export.Initialise();

            Initialise();

            TestButton();
            TestCanvas();
            TestComboBox();
            Test_DDContainer.Test();

            Export.AddFrameDelegate(new MyGUI.Sharp.Demo.Export.HandleFrameStart(FrameStart));
            Export.Run();
        }

        static void TestButton()
        {
            Button button = new Button(IntPtr.Zero, WidgetStyle.Overlapped, "MultiListButton", new IntCoord(10, 10, 100, 100), Align.Default, "Overlapped", "");

            uint index = uint.MaxValue;
            index = button.ImageIndex;
            button.ImageIndex = uint.MaxValue;
            index = button.ImageIndex;
            button.ImageIndex = 0;
            index = button.ImageIndex;

            bool value = false;
            value = button.StateCheck;
            button.StateCheck = false;
            value = button.StateCheck;
            button.StateCheck = true;
            value = button.StateCheck;

            value = false;
            value = button.ButtonPressed;
            button.ButtonPressed = false;
            value = button.ButtonPressed;
            button.ButtonPressed = true;
            value = button.ButtonPressed;

            StaticImage image = button.GetStaticImage();
        }

        static void TestCanvas()
        {
            Canvas canvas = new Canvas(IntPtr.Zero, WidgetStyle.Overlapped, "Canvas", new IntCoord(20, 20, 100, 100), Align.Default, "Overlapped", "");
            //IntSize size = canvas.GetTextureRealSize();
        }

        static void TestComboBox()
        {
            ComboBox box = new ComboBox(IntPtr.Zero, WidgetStyle.Overlapped, "ComboBox", new IntCoord(320, 320, 200, 26), Align.Default, "Overlapped", "");
            box.AddItem("line0", (int)0);
            box.AddItem("line1");
            box.AddItem("line2", (int)2);
            box.AddItem("line3");
            box.AddItem("line4");
            box.AddItem("line5");
            box.AddItem("line6");
            box.AddItem("line7");
            box.AddItem("line8");

            box.MaxListHeight = box.MaxListHeight + 10;
            box.SmoothShow = !box.SmoothShow;
            box.ComboModeDrop = !box.ComboModeDrop;
            box.ComboModeDrop = !box.ComboModeDrop;

            box.BeginToItemSelected();
            box.BeginToItemLast();
            box.BeginToItemFirst();
            box.BeginToItemAt(2);

            string name = box.GetItemNameAt(2);
            box.SetItemNameAt(2, "new line2");
            name = box.GetItemNameAt(2);

            box.SetItemDataAt(0, (int)0);
            object data = box.GetItemDataAt(0);
            box.SetItemDataAt(3, (int)3);
            data = box.GetItemDataAt(3);

            box.ClearItemDataAt(0);
            data = box.GetItemDataAt(0);
            data = box.GetItemDataAt(3);

            box.IndexSelected = 1;
            box.IndexSelected = box.IndexSelected + 1;
            box.ClearIndexSelected();

            uint index = box.FindItemIndexWith("line3");
            box.RemoveItemAt(3);
            index = box.FindItemIndexWith("line3");

            box.InsertItemAt(2, "insert line2", (int)2);
            name = box.GetItemNameAt(2);
            name = box.GetItemNameAt(3);

            uint count = box.ItemCount;

            box.EventComboAccept += new ComboBox.HandleComboAccept(box_EventComboAccept);
            box.EventComboChangePosition += new ComboBox.HandleComboChangePosition(box_EventComboChangePosition);
        }

        static void box_EventComboChangePosition(ComboBox _sender, uint _index)
        {
            Export.DebugOut("EventComboChangePosition  index=" + _index.ToString());
        }

        static void box_EventComboAccept(ComboBox _sender, uint _index)
        {
            Export.DebugOut("EventComboAccept  index=" + _index.ToString());
        }

        static void Initialise()
        {
            /*Button button = Gui.Instance.CreateWidget<Button>("Button", new IntCoord(10, 10, 200, 26), Align.Default, "Main");
            button.Caption = "Button";
            button.EventMouseButtonClick += new Widget.HandleMouseButtonClick(button_EventMouseButtonClick);

            ComboBox combo = Gui.Instance.CreateWidget<ComboBox>("ComboBox", new IntCoord(10, 50, 200, 26), Align.Default, "Main");
            combo.Caption = "ComboBox";
            combo.AddItem("line1", null);
            combo.AddItem("line2", null);
            combo.AddItem("line3", null);

            EditBox edit = Gui.Instance.CreateWidget<EditBox>("Edit", new IntCoord(10, 90, 200, 26), Align.Default, "Main");
            edit.Caption = "Edit";

            ListBox list = Gui.Instance.CreateWidget<ListBox>("List", new IntCoord(10, 130, 200, 126), Align.Default, "Main");
            list.Caption = "List";
            list.AddItem("line1", null);
            list.AddItem("line2", null);
            list.AddItem("line3", null);

            Widget widget = Gui.Instance.CreateWidget<Widget>("Panel", new IntCoord(10, 260, 200, 126), Align.Default, "Main");
            //IntSize size = widget.SubWidgetText.GetTextSize();

            StaticImage image = Gui.Instance.CreateWidget<StaticImage>("StaticImage", new IntCoord(230, 10, 32, 32), Align.Default, "Main");
            image.SetItemResource(new Guid("{C7ECFCE4-6021-4628-9BFF-1F36D594D7CB}"));//"pic_CorePointerArrow");
            image.SetItemGroup("Pointer");
            image.SetItemName("Pointer");

            //Gui.Instance.LoadLayout("Console2.layout", null, "");

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
            /*Button test1;
            Canvas test1_2;
            ComboBox test2;
            DDContainer test3;
            EditBox test4;
            HScrollBar test5;
            ItemBox test6;
            ListBox test7;
            MenuBar test8;
            MenuCtrl test9;
            MessageBox test10;
            MultiListBox test11;
            PopupMenu test12;
            ProgressBar test13;
            RenderBox test14;
            ScrollView test15;
            StaticImage test16;
            StaticText test17;
            TabBar test18;
            VScrollBar test19;
            Widget test20;
            Window test21;

            MenuItem test22;
            TabItem test23;//*/

        }

        /*static void button_EventMouseButtonClick(Widget _sender)
        {
            MessageBox mess = Gui.Instance.CreateWidget<MessageBox>("Message", new IntCoord(0, 0, 0, 0), Align.Default, "Popup");
            mess.SetMessageButton(MessageBoxStyle.Continue);
            //mess.SetMessageImage(MessageBoxStyle.IconInfo);
            mess.EventMessageBoxResult += new MessageBox.HandleMessageBoxResult(mess_EventMessageBoxResult);
        }

        static void mess_EventMessageBoxResult(MessageBox _sender, MessageBoxStyle _result)
        {
            int test = 0;
        }*/

        static void FrameStart(float _time)
        {
            int test = 0;
        }

    }
}
