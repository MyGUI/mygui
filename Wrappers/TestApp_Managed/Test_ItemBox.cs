using System;
using MyGUI.Managed;

namespace TestApp.Managed
{

    public class Test_ItemBox
    {
        public static void Test()
        {
            ItemBox box = Gui.Instance.CreateWidget<ItemBox>("ItemBox", new IntCoord(20, 320, 100, 100), Align.Default, "Main");

            box.EventNotifyItem += new ItemBox.HandleNotifyItem(box_EventNotifyItem);
            box.EventMouseItemActivate += new ItemBox.HandleMouseItemActivate(box_EventMouseItemActivate);
            box.EventChangeItemPosition += new ItemBox.HandleChangeItemPosition(box_EventChangeItemPosition);
            box.EventSelectItemAccept += new ItemBox.HandleSelectItemAccept(box_EventSelectItemAccept);
            box.RequestDrawItem += new ItemBox.HandleDrawItem(box_RequestDrawItem);
            box.RequestCoordItem += new ItemBox.HandleCoordItem(box_RequestCoordItem);
            box.RequestCreateWidgetItem += new ItemBox.HandleCreateWidgetItem(box_RequestCreateWidgetItem);

            box.AddItem("cell0");

            box.ResetDrag();
            Widget cell = box.GetWidgetByIndex(0);
			if (cell != null)
			{
				uint index = box.GetIndexByWidget(cell);
			}
            Widget drag = box.WidgetDrag;
            box.VerticalAlignment = !box.VerticalAlignment;
            box.ClearItemDataAt(0);
            box.SetItemDataAt(0, "new cell0");
            box.ClearIndexSelected();
            box.IndexSelected = 0;
            box.RedrawAllItems();
            box.RedrawItemAt(0);
            box.RemoveItemAt(0);
            box.RemoveAllItems();

            box.AddItem("cell0");
            box.SetItemDataAt(0, "new cell0");
            box.InsertItemAt(0, "insert cell0");
            uint count = box.ItemCount;
        }

        static void box_RequestCreateWidgetItem(ItemBox _sender, Widget _item)
        {
            EditBox cell = _item.CreateWidget<EditBox>("Edit", new IntCoord(0, 0, 50, 50), Align.Default);
            _item.UserData = cell;
        }

        static void box_RequestCoordItem(ItemBox _sender, ref IntCoord _coord, bool _drag)
        {
            _coord.left = 0;
            _coord.top = 0;
            _coord.width = 70;
            _coord.height = 70;
        }

        static void box_RequestDrawItem(ItemBox _sender, Widget _item, IBDrawItemInfo _info)
        {
            EditBox cell = _item.UserData as EditBox;
            string str = _sender.GetItemDataAt(_info.index) as string;
            if (str != null) cell.OnlyText = str;
        }

        static void box_EventSelectItemAccept(ItemBox _sender, uint _index)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventSelectItemAccept  index=" + _index.ToString());
        }

        static void box_EventChangeItemPosition(ItemBox _sender, uint _index)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventChangeItemPosition  index=" + _index.ToString());
        }

        static void box_EventMouseItemActivate(ItemBox _sender, uint _index)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventMouseItemActivate  index=" + _index.ToString());
        }

        static void box_EventNotifyItem(ItemBox _sender, IBNotifyItemData _info)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventNotifyItem  notify=" + _info.notify.ToString());
        }
    }

}
