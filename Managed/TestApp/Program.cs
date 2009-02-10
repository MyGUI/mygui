using System;
using System.Collections.Generic;
using System.Text;
//using MMyGUI;
using RenderFacade.Interfaces.GUI;

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
            IButton button = Gui.Instance.CreateWidget<IButton>("Button", new IntCoord(10, 10, 200, 26), Align.Default, "Main");
            button.Caption = "Button";
            button.EventMouseButtonClick += new IWidget.HandleMouseButtonClick(button_EventMouseButtonClick);

            IComboBox combo = Gui.Instance.CreateWidget<IComboBox>("ComboBox", new IntCoord(10, 50, 200, 26), Align.Default, "Main");
            combo.Caption = "ComboBox";
            combo.AddItem("line1", null);
            combo.AddItem("line2", null);
            combo.AddItem("line3", null);

            IEditBox edit = Gui.Instance.CreateWidget<IEditBox>("Edit", new IntCoord(10, 90, 200, 26), Align.Default, "Main");
            edit.Caption = "Edit";

            IListBox list = Gui.Instance.CreateWidget<IListBox>("List", new IntCoord(10, 130, 200, 126), Align.Default, "Main");
            list.Caption = "List";
            list.AddItem("line1", null);
            list.AddItem("line2", null);
            list.AddItem("line3", null);

            IWidget widget = Gui.Instance.CreateWidget<IWidget>("Panel", new IntCoord(10, 260, 200, 126), Align.Default, "Main");
            //IntSize size = widget.SubWidgetText.GetTextSize();

            IStaticImage image = Gui.Instance.CreateWidget<IStaticImage>("StaticImage", new IntCoord(230, 10, 32, 32), Align.Default, "Main");
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
            IButton test1;
            ICanvas test1_2;
            IComboBox test2;
            IDDContainer test3;
            IEditBox test4;
            IHScrollBar test5;
            IItemBox test6;
            IListBox test7;
            IMenuBar test8;
            IMenuCtrl test9;
            IMessageBox test10;
            IMultiListBox test11;
            IPopupMenu test12;
            IProgressBar test13;
            IRenderBox test14;
            IScrollView test15;
            IStaticImage test16;
            IStaticText test17;
            ITabBar test18;
            IVScrollBar test19;
            IWidget test20;
            IWindow test21;

            IMenuItem test22;
            ITabItem test23;//*/

        }

        static void button_EventMouseButtonClick(IWidget _sender)
        {
            IMessageBox mess = Gui.Instance.CreateWidget<IMessageBox>("Message", new IntCoord(0, 0, 0, 0), Align.Default, "Popup");
            mess.SetMessageButton(MessageBoxStyle.Continue);
            //mess.SetMessageImage(MessageBoxStyle.IconInfo);
            mess.EventMessageBoxResult += new IMessageBox.HandleMessageBoxResult(mess_EventMessageBoxResult);
        }

        static void mess_EventMessageBoxResult(IMessageBox _sender, MessageBoxStyle _result)
        {
            int test = 0;
        }

        static void FrameStart(float _time)
        {
        }

    }
}
